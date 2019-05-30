#include "reduce.h"

/* NOTE: Experimental reduction process.
Picks the best solution and on each iteration adds the solution farthest away from
the already picked ones. */
void reduce_sci(const problem *prob,
        solution **sols, int *n_sols, int target_n){
    // Return if there is no need of reduction.
    if(*n_sols<=target_n) return;
    // Initialize array of distances and bool array for selected solutions
    lint *dists = malloc(sizeof(lint)*(*n_sols));
    char *picked = malloc(sizeof(char)*(*n_sols));
    for(int i=0;i<*n_sols;i++){
        picked[i] = 0;
    }
    int n_picked = 0;
    // Pick solution with more value first, and update min_dists.
    int best_i = 0;
    for(int i=1;i<*n_sols;i++){
        if(sols[best_i]->value<sols[i]->value){
            best_i = i;
        }
    }
    picked[best_i] = 1;
    n_picked += 1;
    for(int i=0;i<*n_sols;i++){
        dists[i] = solution_dissimilitude(prob,sols[i],sols[best_i]);
    }
    // Pick remaining representatives
    for(int k=1;k<target_n;k++){
        int larger_i = -1;
        for(int i=1;i<*n_sols;i++){
            if(!picked[i] && (larger_i==-1 || dists[i]>dists[larger_i])){
                larger_i = i;
            }
        }
        assert(larger_i!=-1);
        picked[larger_i] = 1;
        n_picked += 1;
        for(int i=0;i<*n_sols;i++){
            lint n_dist = solution_dissimilitude(prob,sols[i],sols[larger_i]);
            if(n_dist<dists[i]){
                dists[i] = n_dist;
            }
        }
    }
    // Reduce the solutions array:
    int k=0;
    for(int i=0;i<*n_sols;i++){
        if(picked[i]){
            sols[k++] = sols[i];
        }else{
            free(sols[i]);
        }
    }
    assert(n_picked==k);
    // Set the amount of solutions right.
    if(*n_sols>target_n) *n_sols = target_n;
    // Free arrays
    free(dists);
    free(picked);
}
