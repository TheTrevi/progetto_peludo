#include "minimizer.h"
#include <iostream>

using namespace std;

// Implementazione della discesa del gradiente a passo fisso (step = 0.1).
void mimimizeFunction(const Function& f, const int MAX_ITERATIONS, const double STOP_NORM, const Vector2 START_POS) {
    Vector2 pos = START_POS;
    Vector2 grad;
    
    // Intestazione tabella di convergenza
    cout << "k\tp[k]\tf(p[k])\tgradMod" << endl;

    for (int i = 0; i < MAX_ITERATIONS; i++) {
        // Calcolo del gradiente nel punto corrente
        grad = f.gradient(pos);

        // Controllo del criterio di arresto sul modulo del gradiente
        if (grad.mod() < STOP_NORM) {
            cout << "STOP_NORM_REACHED" << endl;
            cout << i << "\t" << pos << "\t" << f.evaluate(pos) << "\t" << grad.mod() << endl;
            return;
        }

        // Calcolo del versore del gradiente e aggiornamento posizione
        Vector2 norm = grad.norm();
        pos -= norm * 0.1; //f.getStep();

        // Stampa periodica dello stato ogni 10 iterazioni
        if (i % 10 == 0) {
            cout << i << "\t" << pos << "\t" << f.evaluate(pos) << "\t" << grad.mod() << endl;
        }
    }

    // Raggiunto il limite massimo di iterazioni
    cout << "MAX_ITERATIONS_REACHED" << endl;
    cout << MAX_ITERATIONS - 1 << "\t" << pos << "\t" << f.evaluate(pos) << "\t" << grad.mod() << endl;
}
