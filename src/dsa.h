#ifndef DSA_DSA_H
#define DSA_DSA_H

#include "reduce.h"
#include "expand.h"
#include "load.h"

solution **new_find_best_solutions(problem* prob,
        int n_random, int pool_size, int vision_range, int *final_n, int *n_iterations);

void local_search_solutions(problem* prob, solution **sols, int *n_sols);

#endif
