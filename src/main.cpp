#include <iostream>
#include <cmath>
#include <string>
#include "../include/function.h"

using namespace std;

int main() {
    double T;
    int N;

    cout << "Soglia sulla norma del gradiente T: ";
    cin >> T;
    cout << "Numero massimo di iterazioni N: ";
    cin >> N;

    string str_e  = to_string(M_E);
    string str_pi = to_string(M_PI);

    Function f1("((x^2) + (y^2)) / 2");
    Function f2("((x^2) + (16 * (y^2))) / 2");
    Function f3("(((x^2) - 1)^2) + (y^2)");
    Function f4("((((x^2) - 1)^2) + (((x - 1)^2) / 5)) + (y^2)");
    Function f5("(4 * ((sinh(x / 2))^2)) + (y^2)");
    Function f6("log((1 + (x^2)) + (y^2))");
    Function f7("1 - (" + str_e + " ^ (0 - ((x^2) + (y^2))))");
    Function f8("(2 - cos(" + str_pi + " * x)) - cos(" + str_pi + " * y)");

    return 0;
}
