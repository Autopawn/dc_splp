#ifndef DSA_SOLUTION_H
#define DSA_SOLUTION_H

#include "common.h"

typedef struct {
    int n_facilities;
    // ^ Number of facilities in this solution.
    short facilities[MAX_SOL_SIZE];
    // ^ Facilities from the smaller index to the larger one.
    short assignments[MAX_CLIENTS];
    // ^ Which facility is working each client.
    lint value;
    // ^ Value of this solution on the objective function.
} solution;

solution empty_solution();

void solution_add(const problem *prob, solution *sol, short newf);

void solution_remove(const problem *prob, solution *sol, short remf);

lint solution_dissimilitude(const problem *prob,
        const solution *sol_a, const solution *sol_b);

solution solution_hill_climbing(const problem *prob, solution sol);

int solution_value_cmp_inv(const void *a, const void *b);

int solution_cmp(const void *a, const void *b);


#endif
