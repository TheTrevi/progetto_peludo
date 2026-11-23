#ifndef FUNCTION_H
#define FUNCTION_H

#include <cstddef>
#include <limits>
#include <string>
#include "vector2.h"


enum Tipo {NUMERO, VARIABILE, FUNZIONE, OPERATORE};

struct ValGrad {
    double val;   // valore della funzione
    double dx;    // derivata parziale rispetto a x
    double dy;    // derivata parziale rispetto a y
};

// Nodo dell'albero sintattico di una funzione f(x, y).
//   NUMERO:    foglia, contiene il valore in 'numero'
//   VARIABILE: foglia, 'nomeTipo' e' "x" oppure "y"
//   FUNZIONE:  un figlio (leftPtr), 'nomeTipo'
//   OPERATORE: due figli (leftPtr, rightPtr), 'nomeTipo'
// Il distruttore elimina i figli.

class Node {
    public:
        Node(Tipo t, std::string n="", double value=0) : tipo(t), nomeTipo(n), numero(value), leftPtr(nullptr), rightPtr(nullptr) {}
        ~Node() {delete leftPtr; delete rightPtr;}

        // Non copiabile.
        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;

        double calculate(const Vector2& p) const;
        ValGrad calculateGradient(const Vector2 &p) const;
        std::string toString() const;

        void setRightPtr(Node* ptr) {rightPtr = ptr;};
        void setLeftPtr(Node* ptr) {leftPtr = ptr;};

        Node* getRightPtr() const {return rightPtr;};
        Node* getLeftPtr() const {return leftPtr;};

    private:
        Tipo tipo;
        std::string nomeTipo;
        double numero;          // valido solo se tipo == NUMERO
        Node* leftPtr;
        Node* rightPtr;
};

class Intervallo {
    public:
        bool contiene(double v) const {
            bool sopraMin = minIncluso ? v >= min : v > min;
            bool sottoMax = maxIncluso ? v <= max : v < max;
            return sopraMin && sottoMax;
        }

        Intervallo();
        Intervallo(double a, double b, bool minIncluso, bool maxIncluso): min(a), max(b), minIncluso(minIncluso), maxIncluso(maxIncluso) {};

    private:
        double min = -std::numeric_limits<double>::infinity();
        double max =  std::numeric_limits<double>::infinity();
        bool minIncluso = true;
        bool maxIncluso = true;
};

struct Dominio {
    Intervallo x;
    Intervallo y;

    bool contiene(const Vector2& vec) const {
        return x.contiene(vec.x) && y.contiene(vec.y);
    }
};

// Funzione di due variabili f(x, y), costruita a partire da una stringa.
// La precedenza la danno le parentesi.
// esempio ((3 * 2) * (3 - (4 * sin(x)))).
class Function {
    public:
        Function(std::string funzione, Dominio dom, double step=0) : formula(funzione), root(nullptr), dominio(dom), step(step) {parseFullParentesis(funzione);};
        ~Function() {delete root;}

        // NOn si può copiare
        Function(const Function&) = delete;
        Function& operator=(const Function&) = delete;

        std::string toString() const {return root->toString();}

        // Getters and Setters
        Dominio getDominio() const {return dominio;}
        double getStep() const {return step;}

        void setDominio(Dominio dom) {dominio = dom;}
        void setStep(double step) {step = step;};

        // Calcolo valore esatto e gradiente.
        double evaluate(const Vector2& vec) const;
        Vector2 gradient(const Vector2& vec) const;
        Vector2 gradientApprox(const Vector2& vec) const;

    private:
        std::string formula;
        Node* root;
        Dominio dominio;
        double step;

        bool parseFullParentesis(const std::string& funzione);

        // Parsing ricorsivo.
        Node* parseExpression(const std::string& funz, size_t* posizione) const;
        Node* parseOperando(const std::string& funz, size_t* posizione) const;
        Node* parseParentesi(const std::string& funz, size_t* posizione) const;
        Node* parseNumero(const std::string& funz, size_t* posizione, size_t lunghezza) const;
        Node* parseIdentificatore(const std::string& funz, size_t* posizione) const;
        Node* parseFunzione(const std::string& nome, const std::string& funz, size_t* posizione) const;
        // Approssimazione derivate parziali.
        double partialX_approx(const Vector2& p) const;
        double partialY_approx(const Vector2& p) const;
};


#endif
