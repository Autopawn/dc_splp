#ifndef DSA_REDUCE_H
#define DSA_REDUCE_H

#include <pthread.h>

#include "solution.h"
#include "load.h"

#include <pthread.h>
#include <semaphore.h>

void reduce_solutions(const problem *prob,
        solution **sols, int *n_sols, int target_n, int vision_range);

#endif
