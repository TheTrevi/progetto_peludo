#include <iostream>
#include <cmath>
#include <string>
#include "../include/function.h"
#include "../include/minimizer.h"

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

    Function f[8] = {// da aggiungere dominio e step dal pdf
        Function("((x^2) + (y^2)) / 2"),
        Function("((x^2) + (16 * (y^2))) / 2"),
        Function("(((x^2) - 1)^2) + (y^2)"),
        Function("((((x^2) - 1)^2) + (((x - 1)^2) / 5)) + (y^2)"),
        Function("(4 * ((sinh(x / 2))^2)) + (y^2)"),
        Function("log((1 + (x^2)) + (y^2))"),
        Function("1 - (" + str_e + " ^ (0 - ((x^2) + (y^2))))"),
        Function("(2 - cos(" + str_pi + " * x)) - cos(" + str_pi + " * y)")
    };

    Vector2 start_pos(1.0, 1.0); // da acquisire in input

    for (int i = 0; i < 8; i++) {
        cout << "\n--- Minimizzazione f[" << i << "] ---" << endl;
        mimimizeFunction(f[i], N, T, start_pos);
    }

    return 0;
}
