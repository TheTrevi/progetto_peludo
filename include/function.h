#ifndef FUNCTION_H
#define FUNCTION_H


#include <string>


enum Tipo {NUMERO, VARIABILE, FUNZIONE, OPERATORE};


class Node {
    public:
        Node(Tipo t, std::string n="", double value=0) : tipo(t), nomeTipo(n) ,numero(value), leftPtr(nullptr), rightPtr(nullptr) {}

        ~Node() {delete leftPtr; delete rightPtr;}

        double calculate(double x, double y) const;
        std::string toString() const;

        void setRightPtr(Node* ptr) {rightPtr = ptr;};
        void setLeftPtr(Node* ptr) {leftPtr = ptr;};

        Node* getRightPtr() const {return rightPtr;};
        Node* getLeftPtr() const {return leftPtr;};

        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;

    private:
        Tipo tipo;
        std::string nomeTipo; // nome variabile, (sin, cos, ...) se funzione, (+,-,*,/) se un operatore
        double numero; // SE tipo=NUMERO
        Node* leftPtr;
        Node* rightPtr;
};


class Function {
    public:
        Function(std::string funzione): formula(funzione), root(nullptr)  {parseFullParentesis(funzione);};

        std::string toString() const {return  root->toString();}
        bool parseFullParentesis(const std::string& funzione);
        double evaluate(double x, double y) const;
        // double partialX(double x, double y) const;
        // double partialY(double x, double y) const;

        Function(const Function&) = delete;
        Function& operator=(const Function&) = delete;

        ~Function() {delete root;}


    private:
        Node* root;
        std::string formula;

        Node* parseExpression(const std::string& funz, size_t* posizione) const;
        Node* parseTipo(const std::string& funz, size_t* posizione) const;
};






#endif
