#ifndef DSA_REDUCE_H
#define DSA_REDUCE_H

#include <pthread.h>

#include "solution.h"
#include "load.h"

void reduce_random(const problem *prob,
        solution **sols, int *n_sols, int target_n);
void reduce_bests(const problem *prob,
        solution **sols, int *n_sols, int target_n);
void reduce_sci(const problem *prob,
        solution **sols, int *n_sols, int target_n);
void reduce_vr(const problem *prob,
        solution **sols, int *n_sols, int target_n, int vision_range);

void reduce_solutions(const problem *prob,
        solution **sols, int *n_sols, int target_n,
        int n_random, int vision_range);


#endif
