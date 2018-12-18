#include "dsa.h"

solution **new_find_best_solutions(problem* prob,
        int pool_size, int vision_range, int *final_n, int *n_iterations){
    // Place to store all the pools:
    int pools_size[MAX_FACILITIES+1];
    for(int i=0;i<MAX_FACILITIES+1;i++) pools_size[i] = 0;
    solution **pools[MAX_FACILITIES+1];
    for(int i=0;i<MAX_FACILITIES+1;i++) pools[i] = NULL;
    // Create the first pool:
    solution empt = empty_solution();
    solution *pool0[1];
    pool0[0] = &empt;
    pools[0] = pool0;
    pools_size[0] = 1;
    // Create all the next pools:
    *n_iterations = -1;
    int total_pools_size = 0;
    int STEPS = MAX_SOL_SIZE<MAX_FACILITIES? MAX_SOL_SIZE:MAX_FACILITIES;
    for(int i=1;i<=STEPS;i++){
        printf("Expanding %d solutions of size %d...\n",pools_size[i-1],i-1);
        pools[i] = new_expand_solutions(prob, pools[i-1],
            pools_size[i-1], &pools_size[i]);
        #ifdef VERBOSE_BASE
            printf("#BASE %d %d\n",i,pools_size[i]);
            print_solsets(pools[i],pools_size[i]);
        #endif
        if(pools_size[i]==0){
            *n_iterations = i;
            printf("No more valuable solutions of size %d!\n",i);
            break;
        }
        printf("Reducing %d solutions of size %d...\n",pools_size[i],i);
        // Apply reduction on the pool:
        reduce_solutions(prob, pools[i], &pools_size[i],
            pool_size, vision_range);
        // Realloc to reduce memory usage:
        pools[i] = realloc(pools[i],sizeof(solution*)*pools_size[i]);
        //
        #ifdef VERBOSE_POOL
            printf("#POOL %d %d\n",i,pools_size[i]);
            print_solsets(pools[i],pools_size[i]);
        #endif
        total_pools_size += pools_size[i];
        //
        if(prob->size_restriction!=-1 && i==prob->size_restriction){
            printf("size restriction reached.\n");
            *n_iterations = prob->size_restriction;
            break;
        }
        if(i==STEPS){
            printf("MAX_SOL_SIZE reached.\n");
        }
    }
    if(*n_iterations==-1) *n_iterations = MAX_SOL_SIZE;
    //
    if(prob->size_restriction==-1){
        printf("Merging pools...\n");
        // Merge all solution pointers into one final array:
        solution **final = safe_malloc(sizeof(solution*)*total_pools_size);
        int current_sol_n = 0;
        for(int i=1;i<=MAX_FACILITIES;i++){
            for(int j=0;j<pools_size[i];j++){
                final[current_sol_n] = pools[i][j];
                current_sol_n += 1;
            }
            if(pools[i]!=NULL) free(pools[i]);
        }
        assert(current_sol_n==total_pools_size);
        // Sort solution pointers form best to worst value.
        qsort(final,current_sol_n,sizeof(solution*),solution_value_cmp_inv);
        *final_n = current_sol_n;
        // Return it
        return final;
    }else{
        printf("Picking pool of size p=%d ...\n",prob->size_restriction);
        // Free other arrays:
        for(int i=1;i<=MAX_FACILITIES;i++){
            if(pools[i]!=NULL && i!=prob->size_restriction){
                for(int k=0;k<pools_size[i];k++){
                    free(pools[i][k]);
                }
                free(pools[i]);
            }
        }
        // Pick array only for the solutions of size prob->size_restriction
        *final_n = pools_size[prob->size_restriction];
        return pools[prob->size_restriction];
    }
}

void local_search_solutions(problem* prob, solution **sols, int *n_sols){
    if(*n_sols==0) return;
    printf("Performing local search for %d solutions...\n",*n_sols);
    // Perform HC for each solution:
    for(int i=0;i<*n_sols;i++){
        solution enhanced = solution_hill_climbing(prob,*sols[i]);
        #ifdef VERBOSE_LOCAL_SEARCH
            if(sols[i]->value!=enhanced.value){
                printf("sol %d: %lld -> %lld\n",i,sols[i]->value,enhanced.value);
            }
        #endif
        *sols[i] = enhanced;
    }
    printf("Deleting repeated solutions...\n");
    // Sort solution pointers to indenfify repeated solutions (also in decreasing value).
    qsort(sols,*n_sols,sizeof(solution*),solution_cmp);
    // Detect and delete repeated solutions:
    int n_final = 1;
    for(int i=1;i<*n_sols;i++){
        if(solution_cmp(&sols[n_final],&sols[i])!=0){
            *sols[n_final] = *sols[i];
            n_final++;
        }
    }
    for(int k=n_final;k<*n_sols;k++){
        free(sols[k]);
    }
    printf("Local search reduced %d solutions to %d local optima.\n",*n_sols,n_final);
    *n_sols = n_final;
}
