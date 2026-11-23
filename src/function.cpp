#include "../include/function.h"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <map>
#include <stdexcept>
#include <string>


// ===========================================================================
// Utils
// ===========================================================================

namespace {

struct FunctionDerivative {
    double(*function)(double);
    double(*derivative)(double);
};

struct OperationDerivative {
    double(*function)(double, double);
    double(*derivativeX)(double, double);
    double(*derivativeY)(double, double);
};

const std::string operatorSymbols = "+-*/^";


const std::map<std::string, FunctionDerivative> functions = {
    {"sin", {sin, cos}},
    {"cos", {cos, [](double x) {return -sin(x);}}},
    {"log", {log, [](double x) {return 1/x;}}},
    {"sinh", {sinh, cosh}},
    {"cosh", {cosh, sinh}}
};

const std::map<std::string, OperationDerivative> operators = {
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


std::string removeSpaces(const std::string& str) {
    std::string result;
    for (char c : str) {
        if (!std::isspace(static_cast<unsigned char>(c))) result += c;
    }
    return result;
}

// Restituisce quanti caratteri, a partire da start, formano un numero
size_t searchNumber(const std::string& str, size_t start) {
    size_t i = start;
    size_t digits = 0;
    bool dotFound = false;

    if (i < str.length() && (str[i] == '-' || str[i] == '+')) i++;

    while (i < str.length()) {
        char c = str[i];
        if (std::isdigit(static_cast<unsigned char>(c))) {
            digits++;
        } else if ((c == '.' || c == ',') && !dotFound) {
            dotFound = true;
        } else {
            break;
        }
        i++;
    }

    if (digits == 0) return 0;
    return i - start;
}

// Legge una sequenza di lettere a partire da *position e la restituisce,
// facendo avanzare *position.
std::string readName(const std::string& func, size_t* position) {
    std::string name;
    while (*position < func.length() && std::isalpha(static_cast<unsigned char>(func[*position]))) {
        name += func[*position];
        *position += 1;
    }
    return name;
}


void consumeClosingParenthesis(const std::string& func, size_t* position) {
    if (*position >= func.length() || func[*position] != ')')
        throw std::invalid_argument("Parentesi ')' non trovata in posizione " + std::to_string(*position));
    *position += 1;
}

// 2.000000 -> 2
std::string numberToString(double v) {
    std::string s = std::to_string(v);
    size_t dot = s.find('.');
    if (dot != std::string::npos) {
        size_t end = s.find_last_not_of('0');
        if (end == dot) end--;
        s.erase(end + 1);
    }
    return s;
}

}  // namespace


// ===========================================================================
// Node
// ===========================================================================

// Valuta l'albero ricorsivamente, es. (sin(x) + 2)
//  - NUMBER e VARIABLE casi base.
//  - FUNCTION e OPERATOR calcolano prima i figli e poi applicano l'operazione
double Node::calculate(const Vector2& vec) const {
    switch (type) {
        case NUMBER:    // caso base: 2 -> 2
            return number;

        case VARIABLE:  // caso base: x -> vec.x
            return (typeName == "x") ? vec.x : vec.y;

        case FUNCTION: {  // sin(t): calcolo l'argomento e applica sin
            if (!leftPtr) throw std::runtime_error("Funzione senza argomento");
            return functions.at(typeName).function(leftPtr->calculate(vec));
        }

        case OPERATOR: {  // (t + 2): calcolo sx e dx e poi operatore
            if (!leftPtr || !rightPtr) throw std::runtime_error("Operatore senza operandi");
            return operators.at(typeName).function(leftPtr->calculate(vec), rightPtr->calculate(vec));
        }
    }
    throw std::runtime_error("Errore nel calcolo degli operandi");
}

// Come calculate ma ritorna anche il gradiente (valore, dx, dy).
ValGrad Node::calculateGradient(const Vector2 &vec) const {
    switch (type) {
        case NUMBER:    // caso base: costante, gradiente 0
            return ValGrad{number, 0,0};
        case VARIABLE: {  // caso base: x -> (1,0), y -> (0,1)
            return (typeName == "x") ? ValGrad{vec.x, 1.0, 0} : ValGrad{vec.y, 0,1};
        }

        // Regola catena, esempio:
        // grad f(t) -> f' * grad t
        case FUNCTION: {  // es. sin(x*y): prima il gradiente di x*y, poi si calcola la funzione
            if (!leftPtr) throw std::runtime_error("Funzione senza argomento");
            double(*derivative)(double) = functions.at(typeName).derivative;
            double(*function)(double) = functions.at(typeName).function;

            ValGrad innerExp = leftPtr->calculateGradient(vec);

            return ValGrad{
                function(innerExp.val), //f(t)
                derivative(innerExp.val) * innerExp.dx, // dx f(xy) = f'(x,y) * x'
                derivative(innerExp.val) * innerExp.dy, // fy f(xy) = f'(x,y) * y'
            };

        }

        case OPERATOR: {  // es. (x*y) + 2: gradiente di sx e dx e poi operatore.
            if (!leftPtr || !rightPtr) throw std::runtime_error("Funzione senza argomento");

            double(*function)(double, double) = operators.at(typeName).function;
            double(*derivativeX)(double, double) = operators.at(typeName).derivativeX;
            double(*derivativeY)(double, double) = operators.at(typeName).derivativeY;

            ValGrad u = leftPtr->calculateGradient(vec);
            ValGrad v = rightPtr->calculateGradient(vec);

            double dx = 0, dy = 0;


            // Bisogna controllare se le derivate sono nulle, perché per esempio x^2
            // ha come figlio sx: x e dx: 2, quindi le derivate sinistre sono 0 però
            // la derivata sinistra della potenza dy x^y = x^y * log(x) viene calcolata
            // e per valori di x negativi log(x) da NaN che moltiplivato per 0 da NaN quindi
            // tutte le operazioni dopo avrebbero dato NaN invece di 0.
            if (u.dx != 0 || u.dy != 0) {
                double fu = derivativeX(u.val, v.val);
                dx += fu * u.dx;
                dy += fu * u.dy;
            }
            if (v.dx != 0 || v.dy != 0) {
                double fv = derivativeY(u.val, v.val);
                dx += fv * v.dx;
                dy += fv * v.dy;
            }

            return {
                function(u.val, v.val), dx, dy};
                // fu * u.dx + fv * v.dx, // dx f(x,y) = f_x(x,y) * x' + f_y(x,y) * x'
                // fu * u.dy + fv * v.dy, // dx f(x,y) = f_x(x,y) * x' + f_y(x,y) * x'
        }
    }
    throw std::runtime_error("Errore nel calcolo della derivata");
}

// Stampa l'albero con le parentesi, es. (sin(x) + 2)
// Numeri e variabili sono i casi base, gli altri chiamano toString sui figli.
std::string Node::toString() const {
    switch (type) {
        case NUMBER: {  // caso base: 2 -> "2", -2 -> "(-2)"
            std::string text = numberToString(number);
            return (number < 0) ? "(" + text + ")" : text;
        }

        case VARIABLE:  // caso base: x -> "x"
            return typeName;

        case FUNCTION: {  // sin(t): stampa l'argomento tra parentesi
            std::string arg = leftPtr ? leftPtr->toString() : "<mancante>";
            return typeName + "(" + arg + ")";
        }

        case OPERATOR: {  // (t + 2): stampa sx e dx e li unisce con l'operatore
            std::string left  = leftPtr  ? leftPtr->toString()  : "<mancante>";
            std::string right = rightPtr ? rightPtr->toString() : "<mancante>";
            return "(" + left + " " + typeName + " " + right + ")";
        }
    }
    return "<tipo sconosciuto>";
}


// ===========================================================================
// Function: valutazione e derivate
// ===========================================================================

double Function::evaluate(const Vector2& vec) const {
    if (!root) throw std::runtime_error("Funzione non valida o non inizializzata");
    // if (!domain.contains(vec)) throw std::runtime_error("Funzione non definita fuori dall'intervallo");
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
    if (!root) throw std::runtime_error("Funzione non valida o non inizializzata");
    // if (!domain.contains(vec)) throw std::runtime_error("Funzione non definita fuori dall'intervallo");
    ValGrad val = root->calculateGradient(vec);
    return Vector2(val.dx, val.dy);
}

Vector2 Function::gradientApprox(const Vector2& vec) const {
    return Vector2(partialX_approx(vec), partialY_approx(vec));
}


// ===========================================================================
// Function: parsing
// ===========================================================================
//
// Le funzioni del parser si chiamano a vicenda, lo schema è:
//   expression -> operand
//   operand    -> "( expression )" o "numero" o "variabile" o "nome( expression )"
// Ritorna false se ci sono stati errori nel parsing.
bool Function::parseFullParentheses(const std::string& str) {
    delete root;
    root = nullptr;

    const std::string function = removeSpaces(str);
    size_t position = 0;

    try {
        root = parseExpression(function, &position);
        if (position != function.length())
            throw std::invalid_argument("Caratteri in eccesso in posizione " + std::to_string(position));
        return true;
    } catch (const std::exception&) {
        delete root;
        root = nullptr;
        return false;
    }
}


// Funzione "generale" del parser: legge operand e lega le due parti in un nodo OPERATOR.
// Es. "x+2" -> nodo '+' con figli x e 2.
Node* Function::parseExpression(const std::string& func, size_t* position) const {
    Node* leftNode = parseOperand(func, position);

    if (*position >= func.length() || operatorSymbols.find(func[*position]) == std::string::npos)
        return leftNode;

    std::string operationName = func.substr(*position, 1);
    *position += 1;


    Node* rightNode = nullptr;
    try {
        rightNode = parseOperand(func, position);

        Node* rootNode = new Node(OPERATOR, operationName, 0);
        rootNode->setLeftPtr(leftNode);
        rootNode->setRightPtr(rightNode);
        return rootNode;
    } catch (...) {
        delete leftNode;
        delete rightNode;
        throw;
    }
}

// Decide che tipo di operando c'è e delega al pezzetto giusto:
//   "(x+2)"        -> parseParentheses
//   "3.5"          -> parseNumber
//   "x" / "sin(x)" -> parseIdentifier
Node* Function::parseOperand(const std::string& func, size_t* position) const {
    if (*position >= func.length()) throw std::invalid_argument("Errore parseOperand: stringa finita");

    if (func[*position] == '(')
        return parseParentheses(func, position);

    size_t length = searchNumber(func, *position);
    if (length != 0)
        return parseNumber(func, position, length);

    return parseIdentifier(func, position);
}


// Salta la '(' e riparte da parseExpression per leggere quello che c'è dentro
// (qui avviene la ricorsione per le parentesi annidate), poi chiude la ')'.
Node* Function::parseParentheses(const std::string& func, size_t* position) const {
    *position += 1;  // salta '('
    Node* node = parseExpression(func, position);

    try {
        consumeClosingParenthesis(func, position);
    } catch (...) {
        delete node;
        throw;
    }
    return node;
}

Node* Function::parseNumber(const std::string& func, size_t* position, size_t length) const {
    std::string text = func.substr(*position, length);
    for (char& c : text) if (c == ',') c = '.';  // std::stod vuole il punto
    double value = std::stod(text);
    *position += length;
    return new Node(NUMBER, "", value);
}

// Caso base: "x" o "y" diventano nodi VARIABLE.
// Se invece dopo il nome c'è '(' è una funzione, es. "sin(x)" -> parseFunction.
// Un nome seguito da '(' è una funzione, altrimenti deve essere una variabile.
Node* Function::parseIdentifier(const std::string& func, size_t* position) const {
    std::string name = readName(func, position);
    if (name.empty()) throw std::invalid_argument("Carattere inatteso in posizione " + std::to_string(*position));

    if (*position < func.length() && func[*position] == '(')
        return parseFunction(name, func, position);

    if (name == "x" || name == "y")
        return new Node(VARIABLE, name, 0);

    throw std::invalid_argument("Identificatore sconosciuto: " + name);
}

// nomeFunzione '(' espressione ')'
// L'argomento è una expression qualsiasi, es. sin(x+2) o cos(sin(x)), e poi richiama parseExpression.
Node* Function::parseFunction(const std::string& name, const std::string& func, size_t* position) const {
    if (functions.count(name) == 0) throw std::invalid_argument("Funzione sconosciuta: " + name);
    *position += 1;  // salta '('

    Node* argument = parseExpression(func, position);
    consumeClosingParenthesis(func, position);

    Node* functionNode = new Node(FUNCTION, name, 0);
    functionNode->setLeftPtr(argument);
    return functionNode;
}
