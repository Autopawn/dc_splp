#ifndef DSA_LOAD_H
#define DSA_LOAD_H

#include <math.h>
#include "solution.h"

problem *new_problem_load(const char *file);

void save_solutions(const char *file, solution **sols, int n_sols, int tot_n_sols,
        double multiplier,
        const char *input_file, int pool_size, int vision_range,
        float seconds, int n_iterations, float elapsed);

void print_solution(FILE *f, const solution *sol, double multiplier);
void print_solsets(solution **sols, int n_sols);

#endif
