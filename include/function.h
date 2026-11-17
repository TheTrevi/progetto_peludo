#ifndef FUNCTION_H
#define FUNCTION_H


#include <string>


enum Tipo {NUMERO, VARIABILE, FUNZIONE, OPERATORE};


class Node {
    public:
        Node(Tipo t, std::string n="", double value=0) : tipo(t), nomeTipo(n) ,numero(value), leftPtr(nullptr), rightPtr(nullptr) {}

        ~Node() {delete leftPtr; delete rightPtr;}

        double calculate(double x, double y) const;
        // std::string print() const;

    private:
        Tipo tipo;
        std::string nomeTipo; // nome variabile, (sin, cos, ...) se funzione, (+,-,*,/) se un operatore
        double numero; // SE tipo=NUMERO
        Node* rightPtr;
        Node* leftPtr;
};


class Function {
    public:
        bool parseFullParentesis(const std::string& funzione);
        double evaluate(double x, double y) const;
        // double partialX(double x, double y) const;
        // double partialY(double x, double y) const;
        // std::string getName() const;


        ~Function() {delete root;}


    private:
        Node* root;

        Node* parseExpression(const std::string& funz, size_t* posizione) const;
        Node* parseTipo(const std::string& funz, size_t* posizione) const;
};






#endif
