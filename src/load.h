#ifndef DSA_LOAD_H
#define DSA_LOAD_H

#include "solution.h"

problem *new_problem_load(const char *file);

void save_solutions(const char *file, solution **sols, int n_sols,
        const char *input_file, int pool_size, int vision_range,
        float seconds, int n_iterations);

void print_solution(FILE *f, const solution *sol);
void print_solsets(solution **sols, int n_sols);

#endif
