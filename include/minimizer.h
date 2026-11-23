#ifndef MINIMIZER_H
#define MINIMIZER_H

#include "function.h"
#include "vector2.h"

// Esegue la minimizzazione di una funzione f(x, y) tramite metodo di discesa del gradiente.
// Parametri:
//   f:                 funzione obiettivo da minimizzare
//   MAX_ITERATIONS:    numero massimo di iterazioni consentite
//   STOP_NORM:         soglia di arresto sulla norma del gradiente (||grad|| < STOP_NORM)
//   START_POS:         punto iniziale da cui avviare la ricerca
void mimimizeFunction(const Function& f, const int MAX_ITERATIONS, const double STOP_NORM, const Vector2 START_POS);

#endif
