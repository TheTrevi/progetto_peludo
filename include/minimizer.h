#ifndef MINIMIZER_H
#define MINIMIZER_H

#include "function.h"
#include "vector2.h"

void mimimizeFunction(const Function& f, const int MAX_ITERATIONS, const double STOP_NORM, const Vector2 START_POS);

// Alias con valori di default per poter chiamare comodamente minimizer(f[i])
void minimizer(const Function& f, const int MAX_ITERATIONS = 1000, const double STOP_NORM = 1e-4, const Vector2 START_POS = Vector2(1.0, 1.0));

#endif
