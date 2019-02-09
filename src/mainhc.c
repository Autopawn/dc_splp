#include "dsa.h"

// NOTE: This FILE is legacy.

#include <math.h>
#include <time.h>
#include <sys/time.h>

float get_delta_seconds(struct timeval tv1, struct timeval tv2){
    struct timeval tvdiff = {tv2.tv_sec-tv1.tv_sec,tv2.tv_usec-tv1.tv_usec};
    if(tvdiff.tv_usec<0){
        tvdiff.tv_usec += 1000000;
        tvdiff.tv_sec -= 1;
    }
    float secs = (float)(tvdiff.tv_sec)+1e-6*tvdiff.tv_usec;
    return secs;
}

int main(int argc, char **argv){
    int n_sols_mult,max_size,max_to_show;
    const char *input_file = NULL;
    int good = 1;
    if(argc!=6) good = 0;
    if(good){
        if(sscanf(argv[1],"%d",&n_sols_mult)!=1) good = 0;
        if(sscanf(argv[2],"%d",&max_size)!=1) good = 0;
        if(sscanf(argv[3],"%d",&max_to_show)!=1) good = 0;
        input_file = argv[4];
    }
    if(!good){
        printf("Usage: %s <n_sols_mult> <max_size> <max_sols_to_show> <problem_file> <output_file>\n",argv[0]);
        exit(1);
    }
    //
    printf("N sols multiplicator: %d\n",n_sols_mult);
    printf("Max sol size: %d\n",max_size);
    printf("Max solutions to show: %d\n",max_to_show);
    // Load problem file:
    problem *prob = new_problem_load(input_file);
    // Get the solutions:
    printf("Starting search...\n");

    // ---@> Start counting time
    clock_t start = clock();
    struct timeval elapsed_start;
    gettimeofday(&elapsed_start,NULL);

    // Generate initial random solutions:
    int n_sols = 0;
    solution **sols = safe_malloc(sizeof(solution*)*n_sols_mult*max_size*prob->n_facilities);
    for(int i=0;i<max_size;i++){
        int size;
        if(prob->size_restriction==-1){
            // splp problem
            size = i+1;
        }else{
            // p-median problem
            size = prob->size_restriction;
        }
        if(size>prob->n_facilities) break;
        for(int t=0;t<n_sols_mult;t++){
            for(int k=0;k<prob->n_facilities;k++){
                solution *sol = safe_malloc(sizeof(solution));
                *sol = random_solution(prob,size);
                sols[n_sols] = sol;
                n_sols += 1;
            }
        }
    }

    // Perform local search:
    printf("Starting local searchs...\n");
    local_search_solutions(prob,sols,&n_sols);

    // End counting time
    clock_t end = clock();
    struct timeval elapsed_end;
    gettimeofday(&elapsed_end,NULL);
    float seconds = (float)(end - start) / (float)CLOCKS_PER_SEC;
    float elapsed_seconds = get_delta_seconds(elapsed_start,elapsed_end);
    // ---@>

    // Print best solutions
    printf("Best solutions after HC:\n");
    int sols_show = n_sols;
    if(sols_show>max_to_show) sols_show = max_to_show;
    for(int i=0;i<sols_show;i++){
        print_solution(stdout,sols[i],1.0);
    }
    // Update timer
    end = clock();
    seconds = (float)(end - start) / (float)CLOCKS_PER_SEC;
    printf("All done in %f [s]!\n",seconds);
    printf("Saving solutions...\n");
    save_solutions(argv[5],sols,sols_show,n_sols,
        1.0,
        input_file,n_sols_mult,-1,
        seconds,max_size,elapsed_seconds);
    // Free memory
    for(int i=0;i<n_sols;i++){
        free(sols[i]);
    }
    free(sols);
    free(prob);
    return 0;
}
