#ifndef DSA_EXPAND_H
#define DSA_EXPAND_H

#include "solution.h"

solution **new_expand_solutions(const problem *prob,
        solution** sols, int n_sols, int *out_n_sols);

#endif
