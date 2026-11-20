#include "minimizer.h"
#include <iostream>

using namespace std;

void mimimizeFunction(const Function& f, const int MAX_ITERATIONS, const double STOP_NORM, const Vector2 START_POS) {
    Vector2 pos = START_POS;
    
    cout << "k\tp[k]\tf(p[k])\tgradMod" << endl;


    for (int i = 0; i < MAX_ITERATIONS; i++) {
        Vector2 grad = f.gradient(pos);

        if (grad.mod() < STOP_NORM) {
            cout << "STOP_NORM_REACHED" << endl;
            return;
        }

        Vector2 norm = grad.norm();
        pos -= norm * 0.1; //f.getStep();

        if (i % 10 == 0) {
            cout << i << "\t" << pos << "\t" << f.evaluate(pos) << "\t" << grad.mod() << endl;
        }
    }

    cout << "MAX_ITERATIONS_reached" << endl;
}

void minimizer(const Function& f, const int MAX_ITERATIONS, const double STOP_NORM, const Vector2 START_POS) {
    mimimizeFunction(f, MAX_ITERATIONS, STOP_NORM, START_POS);
}
