#include "../include/function.h"
#include <cctype>
#include <map>
#include <cmath>
#include <string>
#include <stdexcept>


double neg(double a) {return -a;}

std::map<std::string, double(*)(double)> funzioni = {
    {"sin", sin}, {"cos", cos}, {"log", log}, {"sinh", sinh}, {"cosh", cosh}
};

double somma(double a, double b) {return a+b;}
double diff(double a, double b) {return a-b;}
double prod(double a, double b) {return a*b;}
double divisione(double a, double b) {return a/b;}

std::map<std::string, double(*)(double, double)> operatori = {
    {"+", somma}, {"-", diff}, {"*", prod}, {"/", divisione}, {"^", pow}
};


// Restituisce quanti caratteri, dall'inizio di str, formano un numero.
size_t searchNumber(const std::string& str) {
    size_t i = 0;
    size_t cifre = 0;
    bool dotFound = false;

    if (i < str.length() && (str[i] == '-' || str[i] == '+')) i++;

    while (i < str.length()) {
        char c = str[i];
        if (std::isdigit(c)) {
            cifre++;
        } else if ((c == '.' || c == ',') && !dotFound) {
            dotFound = true;
        } else {
            break;
        }
        i++;
    }

    if (cifre == 0) return 0;
    return i;
}


double Node::calculate(double x, double y) const {
    if (tipo == NUMERO) return numero;
    switch (tipo) {
        case NUMERO:    return numero;
        case VARIABILE: return (nomeTipo=="x") ? x :y;
        case FUNZIONE: {
            if (!leftPtr) throw std::runtime_error("Funzione senza argomento");
            return funzioni.at(nomeTipo)(leftPtr->calculate(x, y));
        }

        case OPERATORE: {
            if (!leftPtr || !rightPtr) throw std::runtime_error("Operatore senza operandi");
            return operatori.at(nomeTipo)(leftPtr->calculate(x, y), rightPtr->calculate(x, y));
        }
    }
    throw  std::runtime_error("Errore nel calcolo degli operandi");
}


double Function::evaluate(double x, double y) const {
    if (!root) throw std::runtime_error("Funzione non valida o non inizializzata");
    return root->calculate(x, y);
}

bool Function::parseFullParentesis(const std::string& str) {
    // ((3) * (2) ) * ( (3) - ((4) * (sin(x))))
    // Rimuovere spazzi
    std::string funzione="";
    for(size_t i = 0; i< str.length(); i++) {
        if (!::isspace(str[i])) funzione += str[i];
    };

    size_t posizione = 0;
    try {
        root = parseExpression(funzione, &posizione);
        if (root == nullptr)
            throw std::invalid_argument("Espressione vuota");
        if (posizione != funzione.length())
            throw std::invalid_argument("Caratteri in eccesso in posizione " +std::to_string(posizione));
        return true;
    } catch (const std::exception&) {
        delete root;
        root = nullptr;
        return false;
    }
    return true;
};

Node* Function::parseExpression(const std::string& funz, size_t* posizione) const {
    if (*posizione >= funz.length()) throw std::invalid_argument("Errore parseExpression: stringa finita");
    Node* nodoSinistro = parseTipo(funz, posizione);
    if (*posizione < funz.length() && std::string("+-*/^").find(funz[*posizione]) != -1) {
        std::string nomeOperazione = funz.substr(*posizione, 1);
        *posizione += 1;

        Node* nodoDestro = parseTipo(funz, posizione);

        Node* nodoRoot = new Node(OPERATORE, nomeOperazione, 0);
        nodoRoot->setLeftPtr(nodoSinistro);
        nodoRoot->setRightPtr(nodoDestro);
        return nodoRoot;

    }

    return nodoSinistro;
}

Node* Function::parseTipo(const std::string& funz, size_t* posizione) const {
    if (*posizione >= funz.length()) throw std::invalid_argument("Errore parseTipo: stringa finita");


    if (funz[*posizione] == '(') {
        *posizione += 1;
        Node* nodo = parseExpression(funz, posizione);
        if (!nodo) throw std::invalid_argument("Espressione vuota tra parentesi");
        if (*posizione >= funz.length() || funz[*posizione] != ')')
            throw std::invalid_argument("Parentesi ')' non trovata in posizione " + std::to_string(*posizione));
        *posizione += 1;

        return nodo;
    }

    size_t conteggio = searchNumber(funz.substr(*posizione));
    if (conteggio != 0) {
        std::string testo = funz.substr(*posizione, conteggio);
        for (char& c : testo) if (c == ',') c = '.';
        double num = std::stod(testo);
        *posizione += conteggio;
        return new Node(NUMERO, "", num);
    }

    std::string nome = "";

    while (*posizione < funz.length() && std::isalpha(funz[*posizione])) {
        nome += funz[*posizione];
        *posizione += 1;
    }
    if (nome.empty()) throw std::invalid_argument("Carattere inatteso in posizione " + std::to_string(*posizione));


    if (*posizione < funz.length() && funz[*posizione] == '(') {
        if (funzioni.count(nome) == 0) throw std::invalid_argument("Funzione sconosciuta: " + nome);
        *posizione += 1;


        Node* nodo = parseExpression(funz, posizione);

        if (!nodo) throw std::invalid_argument("Argomento vuoto in " + nome);
        if (*posizione >= funz.length() || funz[*posizione] != ')') throw std::invalid_argument("Parentesi ')' non trovata in posizione " + std::to_string(*posizione));
        *posizione += 1;

        Node* nodoFunz(new Node(FUNZIONE, nome, 0));
        nodoFunz->setLeftPtr(nodo);
        return nodoFunz;
    }

    // variabile
    if (nome == "x" || nome == "y")
        return new Node(VARIABILE, nome, 0);

    throw std::invalid_argument("Identificatore sconosciuto: " + nome);
}


static std::string numeroToString(double v) {
    std::string s = std::to_string(v);
    size_t punto = s.find('.');
    if (punto != -1) {
        size_t fine = s.find_last_not_of('0');
        if (fine == punto) fine--;// 2. -> 2
        s.erase(fine + 1);
    }
    return s;
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
