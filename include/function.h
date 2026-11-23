#ifndef FUNCTION_H
#define FUNCTION_H

#include <cstddef>
#include <limits>
#include <string>
#include "vector2.h"


enum Type {NUMBER, VARIABLE, FUNCTION, OPERATOR};

struct ValGrad {
    double val;   // valore della funzione
    double dx;    // derivata parziale rispetto a x
    double dy;    // derivata parziale rispetto a y
};

// Nodo dell'albero sintattico di una funzione f(x, y).
//   NUMBER:   foglia, contiene il valore in 'number'
//   VARIABLE: foglia, 'typeName' e' "x" oppure "y"
//   FUNCTION: un figlio (leftPtr), 'typeName'
//   OPERATOR: due figli (leftPtr, rightPtr), 'typeName'
// Il distruttore elimina i figli.

class Node {
    public:
        Node(Type t, std::string name="", double value=0) : type(t), typeName(name), number(value), leftPtr(nullptr), rightPtr(nullptr) {}
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
        Type type;
        std::string typeName;
        double number;          // valido solo se type == NUMBER
        Node* leftPtr;
        Node* rightPtr;
};

class Interval {
    public:
        bool contains(double v) const {
            bool aboveMin = minIncluded ? v >= min : v > min;
            bool belowMax = maxIncluded ? v <= max : v < max;
            return aboveMin && belowMax;
        }

        Interval() = default;
        Interval(double a, double b, bool minIncluded=true, bool maxIncluded=true): min(a), max(b), minIncluded(minIncluded), maxIncluded(maxIncluded) {};

    private:
        double min = -std::numeric_limits<double>::infinity();
        double max =  std::numeric_limits<double>::infinity();
        bool minIncluded = true;
        bool maxIncluded = true;
};

class Domain {
    public:
        Domain(Interval x, Interval y): x(x), y(y) {};
        Domain(Interval a): x(a), y(a) {};

        bool contains(const Vector2& vec) const {
            return x.contains(vec.x) && y.contains(vec.y);
        }
    private:
        Interval x;
        Interval y;
};

// Funzione di due variabili f(x, y), costruita a partire da una stringa.
// La precedenza la danno le parentesi.
// esempio ((3 * 2) * (3 - (4 * sin(x)))).
class Function {
    public:
        Function(std::string function, Domain dom, double step=0) : formula(function), root(nullptr), domain(dom), step(step) {parseFullParentheses(function);};
        ~Function() {delete root;}

        // NOn si può copiare
        Function(const Function&) = delete;
        Function& operator=(const Function&) = delete;

        std::string toString() const {return root->toString();}

        // Getters and Setters
        Domain getDomain() const {return domain;}
        double getStep() const {return step;}

        void setDomain(Domain dom) {domain = dom;}
        void setStep(double step) {this->step = step;};

        // Calcolo valore esatto e gradiente.
        double evaluate(const Vector2& vec) const;
        Vector2 gradient(const Vector2& vec) const;
        Vector2 gradientApprox(const Vector2& vec) const;

    private:
        std::string formula;
        Node* root;
        Domain domain;
        double step;

        bool parseFullParentheses(const std::string& function);

        // Parsing ricorsivo.
        Node* parseExpression(const std::string& func, size_t* position) const;
        Node* parseOperand(const std::string& func, size_t* position) const;
        Node* parseParentheses(const std::string& func, size_t* position) const;
        Node* parseNumber(const std::string& func, size_t* position, size_t length) const;
        Node* parseIdentifier(const std::string& func, size_t* position) const;
        Node* parseFunction(const std::string& name, const std::string& func, size_t* position) const;
        // Approssimazione derivate parziali.
        double partialX_approx(const Vector2& p) const;
        double partialY_approx(const Vector2& p) const;
};


#endif
