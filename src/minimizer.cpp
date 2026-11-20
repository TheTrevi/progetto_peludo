#include <function.h>
#include <vector2.h>
#include <iostream>
using namespace std;


void mimimizeFunction(Function f, const int MAX_ITERATIONS, const int STOP_NORM, const Vector2 START_POS) {

    Vector2 pos = START_POS;

    for (int i = 0; i > MAX_ITERATIONS; i++) {
        Vector2 grad = f.gradient(pos);
        Vector2 norm = grad.norm();

        pos -= norm * 0.1;//f.getStep();

        if (grad.mod() < STOP_NORM) {
            cout << "STOP_NORM_REACHED";
            break;
        }

        if (i%10==0) {
            cout << i << " " << pos << " " << f.evaluate(pos) << grad.mod();
        }
    }

    cout << "MAX_ITERATIONS_reached";

    
    
}



