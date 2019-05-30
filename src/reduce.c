#include "reduce.h"

// Pick the target_n best solutions.
void reduce_bests(const problem *prob,
        solution **sols, int *n_sols, int target_n){
    // Sort solution pointers from larger to smaller value of the solution.
    qsort(sols,*n_sols,sizeof(solution*),solution_value_cmp_inv);
    // Free other solutions:
    for(int i=target_n;i<*n_sols;i++){
        free(sols[i]);
    }
    // Set the amount of solutions right.
    if(*n_sols>target_n) *n_sols = target_n;
}

// Pick target_n solutions at random, discarding the others.
void reduce_random(const problem *prob,
        solution **sols, int *n_sols, int target_n){
    if(*n_sols>target_n){
        // Put target_n randomly selected solutions first on the array:
        for(int i=0;i<target_n;i++){
            int choice = i+rand()%(*n_sols-i);
            solution *aux = sols[i];
            sols[i] = sols[choice];
            sols[choice] = aux;
        }
    }
    // Free other solutions:
    for(int i=target_n;i<*n_sols;i++){
        free(sols[i]);
    }
    // Set the amount of solutions right.
    if(*n_sols>target_n) *n_sols = target_n;
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@
// MAIN REDUCTION METHOD
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@

#ifdef REDUCTION_RANDOM
void reduce_solutions(const problem *prob,
        solution **sols, int *n_sols, int target_n,
        int n_random, int vision_range){
    assert(n_random==0);
    assert(vision_range==0);
    reduce_random(prob,sols,n_sols,target_n);
}
#endif

#ifdef REDUCTION_BESTS
void reduce_solutions(const problem *prob,
        solution **sols, int *n_sols, int target_n,
        int n_random, int vision_range){
    //
    assert(vision_range==0);
    if(n_random<target_n) n_random=target_n;
    if(n_random>0 && *n_sols>n_random){
        reduce_random(prob,sols,n_sols,n_random);
    }
    reduce_bests(prob,sols,n_sols,target_n);
}
#endif

#ifdef REDUCTION_SCI
void reduce_solutions(const problem *prob,
        solution **sols, int *n_sols, int target_n,
        int n_random, int vision_range){
    //
    assert(vision_range==0);
    if(n_random<target_n) n_random=target_n;
    if(n_random>0 && *n_sols>n_random){
        reduce_random(prob,sols,n_sols,n_random);
    }
    reduce_sci(prob,sols,n_sols,target_n);
}
#endif

#ifdef REDUCTION_HYBRID
void reduce_solutions(const problem *prob,
        solution **sols, int *n_sols, int target_n,
        int n_random, int vision_range){
    if(n_random<target_n) n_random=target_n;
    if(n_random>0 && *n_sols>n_random){
        reduce_random(prob,sols,n_sols,n_random);
    }
    if(*n_sols>target_n){
        reduce_vr(prob,sols,n_sols,target_n,vision_range);
    }
}
#endif
