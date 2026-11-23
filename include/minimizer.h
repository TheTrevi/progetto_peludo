#ifndef MINIMIZER_H
#define MINIMIZER_H

#include "function.h"
#include "vector2.h"

void mimimizeFunction(const Function& f, const int MAX_ITERATIONS, const double STOP_NORM, const Vector2 START_POS);

#endif
