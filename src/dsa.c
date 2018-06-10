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
        #ifdef VERBOSE
            printf("#BASE %d %d\n",i,pools_size[i]);
            print_solsets(pools[i],pools_size[i]);
        #endif
        if(pools_size[i]==0){
            *n_iterations = i;
            printf("No more valuable solutions of size %d!\n",i);
            break;
        }
        printf("Reducing %d solutions of size %d...\n",pools_size[i],i);
        #ifndef EXTENSIVE_VR_TEST_STEP
            // Apply reduction on the pool:
            reduce_solutions(prob, pools[i], &pools_size[i],
                pool_size, vision_range);
        #else
            // Create a copy of the whole pool.
            int sols_size_c = pools_size[i];
            solution **sols_c = safe_malloc(sizeof(solution*)*sols_size_c);
            for(int k=0;k<sols_size_c;k++){
                sols_c[k] = safe_malloc(sizeof(solution));
                memcpy(sols_c[k],pools[i][k],sizeof(solution));
            }
            // Apply reduction on the pool:
            reduce_solutions(prob, pools[i], &pools_size[i],
                pool_size, vision_range);
            //Perform tests for several vision ranges:
            int vrmax = vision_range;
            #ifdef EXTENSIVE_VR_TEST_MAX
                if(vrmax>EXTENSIVE_VR_TEST_MAX) vrmax = EXTENSIVE_VR_TEST_MAX;
            #endif
            for(int p=0; p<vrmax; p+=EXTENSIVE_VR_TEST_STEP){
                // ^ Repeat for several vision ranges.
                if(p==1) continue;
                int vision_range_x = (p==0)? 1:p;
                int sols_size_x = sols_size_c;
                // Create a copy (x) of the whole pool.
                solution **sols_x = safe_malloc(sizeof(solution*)*sols_size_c);
                for(int k=0;k<sols_size_x;k++){
                    sols_x[k] = safe_malloc(sizeof(solution));
                    memcpy(sols_x[k],sols_c[k],sizeof(solution));
                }
                // Reduce the copy (x) with the vrange:
                reduce_solutions(prob,sols_x, &sols_size_x,
                    pool_size, vision_range_x);
                // Compute how many of the solutions with the vision_range
                // remained on the reduction with vision_range_x:
                int remained = 0;
                assert(pools_size[i]==sols_size_x);
                int p_start_x = 0;
                for(int k=0;k<pools_size[i];k++){
                    int r = p_start_x;
                    while(r<sols_size_x){
                        if(sols_x[r]->value>pools[i][k]->value){
                            p_start_x += 1;
                            assert(p_start_x==r+1);
                        }else if(sols_x[r]->value<pools[i][k]->value){
                            break;
                        }
                        if(solution_dissimilitude(prob,
                                pools[i][k],sols_x[r])==0){
                            remained += 1;
                        }
                        r+=1;
                    }
                }
                // Free solutions of copy(x):
                for(int k=0;k<sols_size_x;k++){
                    free(sols_x[k]);
                }
                free(sols_x);
                float remradio = (float)remained/(float)sols_size_x;
                printf("#REMAINED %d %d from %d to %d with vr %d radio %.5f\n",
                    i,remained,sols_size_c,sols_size_x,vision_range_x,remradio);
            }
            // Free copy of the whole pool.
            for(int k=0;k<sols_size_c;k++){
                free(sols_c[k]);
            }
            free(sols_c);
        #endif
        // Realloc to reduce memory usage:
        pools[i] = realloc(pools[i],sizeof(solution*)*pools_size[i]);
        //
        #ifdef VERBOSE
            printf("#POOL %d %d\n",i,pools_size[i]);
            print_solsets(pools[i],pools_size[i]);
        #endif
        total_pools_size += pools_size[i];
        //
        if(i==STEPS){
            printf("MAX_SOL_SIZE reached.\n");
        }
    }
    if(*n_iterations==-1) *n_iterations = MAX_SOL_SIZE;
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
}

void local_search_solutions(problem* prob, solution **sols, int *n_sols){
    printf("Performing local search for %d solutions...\n",*n_sols);
    // Perform HC for each solution:
    for(int i=0;i<*n_sols;i++){
        solution enhanced = solution_hill_climbing(prob,*sols[i]);
        if(sols[i]->value!=enhanced.value){
            printf("sol %d: %lld -> %lld\n",i,sols[i]->value,enhanced.value);
        }
        *sols[i] = enhanced;
    }
    printf("Deleting repeated solutions...\n");
    // Sort solution pointers to indenfify repeated solutions (also in decreasing value).
    qsort(sols,*n_sols,sizeof(solution*),solution_cmp);
    // Detect and delete repeated solutions:
    int n_final = 0;
    for(int i=1;i<*n_sols;i++){
        if(solution_cmp(&sols[n_final],&sols[i])!=0){
            n_final++;
            *sols[n_final] = *sols[i];
        }
    }
    for(int k=n_final;k<*n_sols;k++){
        free(sols[k]);
    }
    printf("Local search reduced %d solutions to %d local optima.\n",*n_sols,n_final);
    *n_sols = n_final;
}
