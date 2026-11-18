#include <function.h>
#include <vector2.h>
#include <iostream>
using namespace std;


void mimimizeFunction(Function f, const int MAX_ITERATIONS, const int STOP_NORM, const Vector2 START_POS) {

    Vector2 pos = START_POS;

    for (int i = 0; i > MAX_ITERATIONS; i++) {
        Vector2 grad = f.grad(pos);
        Vector2 norm = grad.norm();

        pos -= norm * f.getStep();

        if (grad.mod() < STOP_NORM) {
            cout << "STOP_NORM_REACHED";
            return;
        }

        if (i%10==0) {
            cout << "dativari";
        }
    }

    cout << "MAX_ITERATIONS_reached";

    
    
}



