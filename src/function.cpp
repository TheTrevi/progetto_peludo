#include "../include/function.h"
#include <map>
#include <cmath>
#include <string>

double neg(double a) {return -a;}

std::map<std::string, double(*)(double)> funzioni = {
    {"sin", sin}, {"cos", cos}, {"log", log}, {"sinh", sinh}, {"cosh", cos}, {"neg", neg}
};

double somma(double a, double b) {return a+b;}
double diff(double a, double b) {return a-b;}
double prod(double a, double b) {return a*b;}
double div(double a, double b) {return a/b;}

std::map<std::string, double(*)(double, double)> operatori = {
    {"+", somma}, {"-", diff}, {"*", prod}, {"/", div}, {"pow", pow}, {"^", pow}
};


size_t searchNumber(const std::string& str) {
    if (str.empty()) return 0;

    size_t i = 0;
    bool signFound = false;
    bool dotFound = false;
    size_t count = 0;

    if (str[i] == '-' || str[i] == '+') {
        signFound = true;
        i++;
    }

    while (i < str.length()) {
        char c = str[i];

        if (std::isdigit(c)) {
            count++;
        }
        else if ((c == '.' || c == ',') && !dotFound) {
            dotFound = true;
        }else {throw "Numero non valido";}
            i++;
        }

    return count;
}

double Node::calculate(double x, double y) const {
    if (tipo == NUMERO) return numero;

    switch (tipo) {
        case NUMERO:    return numero;
        case VARIABILE: {if (nomeTipo=="x") return x; return y;};
        case FUNZIONE:  {
           double(*func)(double) = funzioni[nomeTipo];
           func(leftPtr->calculate(x, y));
        };
        case OPERATORE: {
            double(*func)(double, double) = operatori[nomeTipo];
            func(leftPtr->calculate(x, y), rightPtr->calculate(x, y));
        };
    }

   return 0;
}


bool Function::parseFullParentesis(const std::string& str) {
    // ((3) * (2) ) * ( (3) - ((4) * (sin(x))))
    // Rimuovere spazzi
    std::string funzione="";
    for(size_t i = 0; i< str.length(); i++) {
        if (!::isspace(str[i])) funzione += str[i];
    };

    size_t posizione=0;
    root = parseExpression(funzione, &posizione);
    //((3)*(2))*((3)-((4)*(sin(x))))

    return true;
};

Node* Function::parseExpression(const std::string& funz, size_t* posizione) const {
    return nullptr;
}

Node* Function::parseTipo(const std::string& funz, size_t* posizione) const {
    if (*posizione >= funz.length()) throw "Errore parseTipo: stringa finita";

    if (funz[*posizione] == '(') {
        *posizione += 1;
        Node* nodo = parseExpression(funz, posizione);
        if (*posizione >= funz.length() || funz[*posizione]) throw "Errore parentesi di chiusura ) non trovata in posizione "; //TODO aggiungere posizione;
        *posizione += 1;
        return nodo;
    }

    size_t conteggio = searchNumber(funz.substr(*posizione));
    if (conteggio != 0) {
        double num = std::stod(funz.substr(*posizione, conteggio));
        *posizione += conteggio;
        return new Node(NUMERO, nullptr, num);
    }

}


// Se è una cifra, un . o un -: leggi il numero intero, avanza di quanti caratteri hai letto e ritorna un nodo NUMERO.
// Se è una lettera:
// Leggi tutte le lettere consecutive: questo è il nome.
// Guarda il carattere dopo.
// Se è (, è una funzione. Avanza, chiama parseExpr() per l’argomento, controlla ), avanza e ritorna un nodo FUNZIONE con l’argomento come figlio.
// Se non è ( e il nome è x o y, ritorna un nodo VARIABILE.
// Altrimenti, errore.
// Qualsiasi altro carattere: errore.
