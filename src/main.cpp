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

    Domain domain1 = Domain{Interval(-4,4)};
    Domain domain3 = Domain{Interval(-1.5,1.5)};
    Domain domain2 = Domain{Interval(-2,2)};


    Function f[8] = {// da aggiungere step dal pdf
        Function("((x^2) + (y^2)) / 2", domain1, 1.0/5),
        Function("((x^2) + (16 * (y^2))) / 2", domain1, 2.0/25),
        Function("(((x^2) - 1)^2) + (y^2)", domain2, 1.0/20),
        Function("((((x^2) - 1)^2) + (((x - 1)^2) / 5)) + (y^2)", domain2, 1.0/20),
        Function("(4 * ((sinh(x / 2))^2)) + (y^2)", domain3, 3.0/20),
        Function("log((1 + (x^2)) + (y^2))", domain1, 3.0/10),
        Function("1 - (" + str_e + " ^ (0 - ((x^2) + (y^2))))", domain2, 1.0/5),
        Function("(2 - cos(" + str_pi + " * x)) - cos(" + str_pi + " * y)", domain1, 1.0/10),
    };

    Vector2 start_pos(1.0, 1.0); // da acquisire in input

    for (int i = 0; i < 8; i++) {
        cout << "\n--- Minimizzazione f[" << i << "] ---" << endl;
        mimimizeFunction(f[i], N, T, start_pos);
    }

    return 0;
}
