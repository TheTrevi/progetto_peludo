#include "../include/function.h"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <map>
#include <stdexcept>
#include <string>


// ===========================================================================
// Tabelle di funzioni e operatori supportati
// ===========================================================================

namespace {

struct funzDerivata {
    double(*funzione)(double);
    double(*derivata)(double);
};

struct operazioneDerivata {
    double(*funzione)(double, double);
    double(*derivataX)(double, double);
    double(*derivataY)(double, double);
};

const std::string simboliOperatori = "+-*/^";


const std::map<std::string, funzDerivata> funzioni = {
    {"sin", {sin, cos}},
    {"cos", {cos, [](double x) {return -sin(x);}}},
    {"log", {log, [](double x) {return 1/x;}}},
    {"sinh", {sinh, cosh}},
    {"cosh", {cosh, sinh}}
};

const std::map<std::string, operazioneDerivata> operatori = {
    {"+", {
        [](double x, double y) {return x+y;},
        [](double, double) {return 1.0;},
        [](double, double) {return 1.0;},
    }},
    {"-", {
        [](double x, double y) {return x-y;},
        [](double, double) {return 1.0;},
        [](double, double) {return -1.0;},
    }},
    {"*", {
        [](double x, double y) {return x*y;},
        [](double, double y) {return y;}, // dx x*y = y
        [](double x, double) {return x;}, // dy x*y = x
    }},
    {"/", {
        [](double x, double y) {return x/y;},
        [](double, double y) {return 1/y;}, // dx x/y = 1/y
        [](double x, double y) {return -(x / pow(y,2));}, // dy x/y = -(x/y^2)
    }},
    {"^", {
        [](double x, double y) {return pow(x,y);},
        [](double x, double y) {return y * pow(x,y-1);}, // dx x^y = y*x^(y-1)
        [](double x, double y) {return pow(x,y) * log(x);}, // dy x^y = x^y * log(xi)
    }}
};


std::string rimuoviSpazi(const std::string& str) {
    std::string risultato;
    for (char c : str) {
        if (!std::isspace(static_cast<unsigned char>(c))) risultato += c;
    }
    return risultato;
}

// Restituisce quanti caratteri, a partire inizio, formano un numero
size_t searchNumber(const std::string& str, size_t inizio) {
    size_t i = inizio;
    size_t cifre = 0;
    bool dotFound = false;

    if (i < str.length() && (str[i] == '-' || str[i] == '+')) i++;

    while (i < str.length()) {
        char c = str[i];
        if (std::isdigit(static_cast<unsigned char>(c))) {
            cifre++;
        } else if ((c == '.' || c == ',') && !dotFound) {
            dotFound = true;
        } else {
            break;
        }
        i++;
    }

    if (cifre == 0) return 0;
    return i - inizio;
}

// Legge una sequenza di lettere a partire da *posizione e la restituisce,
// facendo avanzare *posizione.
std::string leggiNome(const std::string& funz, size_t* posizione) {
    std::string nome;
    while (*posizione < funz.length() && std::isalpha(static_cast<unsigned char>(funz[*posizione]))) {
        nome += funz[*posizione];
        *posizione += 1;
    }
    return nome;
}


void consumaParentesiChiusa(const std::string& funz, size_t* posizione) {
    if (*posizione >= funz.length() || funz[*posizione] != ')')
        throw std::invalid_argument("Parentesi ')' non trovata in posizione " + std::to_string(*posizione));
    *posizione += 1;
}

// 2.000000 -> 2
std::string numeroToString(double v) {
    std::string s = std::to_string(v);
    size_t punto = s.find('.');
    if (punto != std::string::npos) {
        size_t fine = s.find_last_not_of('0');
        if (fine == punto) fine--;
        s.erase(fine + 1);
    }
    return s;
}

}  // namespace


// ===========================================================================
// Node
// ===========================================================================

double Node::calculate(const Vector2& vec) const {
    switch (tipo) {
        case NUMERO:
            return numero;

        case VARIABILE:
            return (nomeTipo == "x") ? vec.x : vec.y;

        case FUNZIONE: {
            if (!leftPtr) throw std::runtime_error("Funzione senza argomento");
            return funzioni.at(nomeTipo).funzione(leftPtr->calculate(vec));
        }

        case OPERATORE: {
            if (!leftPtr || !rightPtr) throw std::runtime_error("Operatore senza operandi");
            return operatori.at(nomeTipo).funzione(leftPtr->calculate(vec), rightPtr->calculate(vec));
        }
    }
    throw std::runtime_error("Errore nel calcolo degli operandi");
}

ValGrad Node::calculateGradient(const Vector2 &vec) const {
    switch (tipo) {
        case NUMERO:
            return ValGrad{numero, 0,0};
        case VARIABILE: {
            return (nomeTipo == "x") ? ValGrad{vec.x, 1.0, 0} : ValGrad{vec.y, 0,1};
        }

        // Regola catena, esempio:
        // grad f(t) -> f' * grad t
        case FUNZIONE: {
            if (!leftPtr) throw std::runtime_error("Funzione senza argomento");
            double(*derivata)(double) = funzioni.at(nomeTipo).derivata;
            double(*funzione)(double) = funzioni.at(nomeTipo).funzione;

            ValGrad expInterna = leftPtr->calculateGradient(vec);

            return ValGrad{
                funzione(expInterna.val), //f(t)
                derivata(expInterna.val) * expInterna.dx, // dx f(xy) = f'(x,y) * x'
                derivata(expInterna.val) * expInterna.dy, // fy f(xy) = f'(x,y) * y'
            };

        }

        case OPERATORE: {
            if (!leftPtr || !rightPtr) throw std::runtime_error("Funzione senza argomento");

            double(*funzione)(double, double) = operatori.at(nomeTipo).funzione;
            double(*derivataX)(double, double) = operatori.at(nomeTipo).derivataX;
            double(*derivataY)(double, double) = operatori.at(nomeTipo).derivataY;

            ValGrad u = leftPtr->calculateGradient(vec);
            ValGrad v = rightPtr->calculateGradient(vec);

            double dx = 0, dy = 0;


            // Bisogna controllare se le derivate sono nulle, perché per esempio x^2
            // ha come figlio sx: x e dx: 2, quindi le derivate sinistre sono 0 però
            // la derivata sinistra della potenza dy x^y = x^y * log(x) viene calcolata
            // e per valori di x negativi log(x) da NaN che moltiplivato per 0 da NaN quindi
            // tutte le operazioni dopo avrebbero dato NaN invece di 0.
            if (u.dx != 0 || u.dy != 0) {
                double fu = derivataX(u.val, v.val);
                dx += fu * u.dx;
                dy += fu * u.dy;
            }
            if (v.dx != 0 || v.dy != 0) {
                double fv = derivataY(u.val, v.val);
                dx += fv * v.dx;
                dy += fv * v.dy;
            }

            return {
                funzione(u.val, v.val), dx, dy};
                // fu * u.dx + fv * v.dx, // dx f(x,y) = f_x(x,y) * x' + f_y(x,y) * x'
                // fu * u.dy + fv * v.dy, // dx f(x,y) = f_x(x,y) * x' + f_y(x,y) * x'
        }
    }
    throw std::runtime_error("Errore nel calcolo della derivata");
}

std::string Node::toString() const {
    switch (tipo) {
        case NUMERO: {
            std::string testo = numeroToString(numero);
            return (numero < 0) ? "(" + testo + ")" : testo;
        }

        case VARIABILE:
            return nomeTipo;

        case FUNZIONE: {
            std::string arg = leftPtr ? leftPtr->toString() : "<mancante>";
            return nomeTipo + "(" + arg + ")";
        }

        case OPERATORE: {
            std::string sx = leftPtr  ? leftPtr->toString()  : "<mancante>";
            std::string dx = rightPtr ? rightPtr->toString() : "<mancante>";
            return "(" + sx + " " + nomeTipo + " " + dx + ")";
        }
    }
    return "<tipo sconosciuto>";
}


// ===========================================================================
// Function: valutazione e derivate
// ===========================================================================

double Function::evaluate(const Vector2& vec) const {
    if (!root) throw std::runtime_error("Funzione non valida o non inizializzata");
    return root->calculate(vec);
}

double Function::partialX_approx(const Vector2& vec) const {
    const double h = 1e-5 * std::max(1.0, std::fabs(vec.x));
    return (evaluate(vec + Vector2(h, 0)) - evaluate(vec - Vector2(h, 0))) / (2 * h);
}

double Function::partialY_approx(const Vector2& vec) const {
    const double h = 1e-5 * std::max(1.0, std::fabs(vec.y));
    return (evaluate(vec + Vector2(0, h)) - evaluate(vec - Vector2(0, h))) / (2 * h);
}

Vector2 Function::gradient(const Vector2& vec) const {
    ValGrad val = root->calculateGradient(vec);
    return Vector2(val.dx, val.dy);
}

Vector2 Function::gradientApprox(const Vector2& vec) const {
    return Vector2(partialX_approx(vec), partialY_approx(vec));
}


// ===========================================================================
// Function: parsing
// ===========================================================================

bool Function::parseFullParentesis(const std::string& str) {
    delete root;
    root = nullptr;

    const std::string funzione = rimuoviSpazi(str);
    size_t posizione = 0;

    try {
        root = parseExpression(funzione, &posizione);
        if (posizione != funzione.length())
            throw std::invalid_argument("Caratteri in eccesso in posizione " + std::to_string(posizione));
        return true;
    } catch (const std::exception&) {
        delete root;
        root = nullptr;
        return false;
    }
}


Node* Function::parseExpression(const std::string& funz, size_t* posizione) const {
    Node* nodoSinistro = parseOperando(funz, posizione);

    if (*posizione >= funz.length() || simboliOperatori.find(funz[*posizione]) == std::string::npos)
        return nodoSinistro;

    std::string nomeOperazione = funz.substr(*posizione, 1);
    *posizione += 1;


    Node* nodoDestro = nullptr;
    try {
        nodoDestro = parseOperando(funz, posizione);

        Node* nodoRoot = new Node(OPERATORE, nomeOperazione, 0);
        nodoRoot->setLeftPtr(nodoSinistro);
        nodoRoot->setRightPtr(nodoDestro);
        return nodoRoot;
    } catch (...) {
        delete nodoSinistro;
        delete nodoDestro;
        throw;
    }
}

Node* Function::parseOperando(const std::string& funz, size_t* posizione) const {
    if (*posizione >= funz.length()) throw std::invalid_argument("Errore parseOperando: stringa finita");

    if (funz[*posizione] == '(')
        return parseParentesi(funz, posizione);

    size_t lunghezza = searchNumber(funz, *posizione);
    if (lunghezza != 0)
        return parseNumero(funz, posizione, lunghezza);

    return parseIdentificatore(funz, posizione);
}


Node* Function::parseParentesi(const std::string& funz, size_t* posizione) const {
    *posizione += 1;  // salta '('
    Node* nodo = parseExpression(funz, posizione);

    try {
        consumaParentesiChiusa(funz, posizione);
    } catch (...) {
        delete nodo;  // manca ')' e quindi elimino l'albero già creato.
        throw;
    }
    return nodo;
}

Node* Function::parseNumero(const std::string& funz, size_t* posizione, size_t lunghezza) const {
    std::string testo = funz.substr(*posizione, lunghezza);
    for (char& c : testo) if (c == ',') c = '.';  // std::stod vuole il punto
    double valore = std::stod(testo);
    *posizione += lunghezza;
    return new Node(NUMERO, "", valore);
}

// Un nome seguito da '(' e' una funzione, altrimenti deve essere una variabile.
Node* Function::parseIdentificatore(const std::string& funz, size_t* posizione) const {
    std::string nome = leggiNome(funz, posizione);
    if (nome.empty()) throw std::invalid_argument("Carattere inatteso in posizione " + std::to_string(*posizione));

    if (*posizione < funz.length() && funz[*posizione] == '(')
        return parseFunzione(nome, funz, posizione);

    if (nome == "x" || nome == "y")
        return new Node(VARIABILE, nome, 0);

    throw std::invalid_argument("Identificatore sconosciuto: " + nome);
}

// nomeFunzione '(' espressione ')'
Node* Function::parseFunzione(const std::string& nome, const std::string& funz, size_t* posizione) const {
    if (funzioni.count(nome) == 0) throw std::invalid_argument("Funzione sconosciuta: " + nome);
    *posizione += 1;  // salta '('

    Node* argomento = parseExpression(funz, posizione);
    consumaParentesiChiusa(funz, posizione);

    Node* nodoFunz = new Node(FUNZIONE, nome, 0);
    nodoFunz->setLeftPtr(argomento);
    return nodoFunz;
}
