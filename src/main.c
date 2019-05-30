#include "dsa.h"

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
    int n_random,pool_size,vision_range,max_to_show;
    int good = 1;
    //
    const char *input_file = NULL;
    const char *output_file_1 = NULL;
    const char *output_file_2 = NULL;
    if(argc!=8) good = 0;
    if(good){
        if(sscanf(argv[1],"%d",&n_random)!=1) good = 0;
        if(sscanf(argv[2],"%d",&pool_size)!=1) good = 0;
        if(sscanf(argv[3],"%d",&vision_range)!=1) good = 0;
        if(sscanf(argv[4],"%d",&max_to_show)!=1) good = 0;
        input_file = argv[5];
        output_file_1 = argv[6];
        output_file_2 = argv[7];
    }
    if(!good){
        printf("Usage: %s <n_random> <pool_size> <vision_range> <max_sols_to_show> <problem_file> <output_before_ls> <output_after_ls>\n",argv[0]);
        printf("A pool_size of 1 will activate greedy mode.\n");
        #ifdef REDUCTION_RANDOM
            printf("A n_random of 0 should be used for this mode. \n");
        #endif
    }
    #ifdef REDUCTION_RANDOM
        if(n_random!=0){
            printf("A n_random of 0 should be used for this mode.\n");
            exit(1);
        }
    #endif
    #if defined(REDUCTION_SCI) || defined(REDUCTION_BESTS) || defined(REDUCTION_RANDOM)
        if(vision_range!=0){
            printf("A vision_range of 0 should be used for this mode.\n");
            exit(1);
        }
    #endif
    #if defined(REDUCTION_RANDOM)
        printf("All random.\n");
    #else
        if(n_random==0){
            printf("Never pick at random.\n");
        }else{
            printf("Pick at %d at random.\n",n_random);
        }
    #endif
    printf("N random: %d\n",n_random);
    assert(n_random>=0);
    printf("Pool size: %d\n",pool_size);
    assert(vision_range>=0);
    printf("Vision range: %d\n",vision_range);
    printf("Max solutions to show: %d\n",max_to_show);

    // ---@> Start counting time
    clock_t start = clock();
    struct timeval elapsed_start;
    gettimeofday(&elapsed_start,NULL);

    // Load problem file:
    problem *prob = new_problem_load(input_file);
    // Get the solutions:
    int n_sols, max_size_found;
    printf("Starting search...\n");

    //
    solution **sols = new_find_best_solutions(prob,
        n_random, pool_size, vision_range, &n_sols, &max_size_found);

    // End counting time
    clock_t end = clock();
    struct timeval elapsed_end;
    gettimeofday(&elapsed_end,NULL);
    float seconds = (float)(end - start) / (float)CLOCKS_PER_SEC;
    float elapsed_seconds = get_delta_seconds(elapsed_start,elapsed_end);
    // ---@>

    printf("Max size solution found: %d\n",max_size_found);
    printf("Search done in %f [s]!\n",seconds);
    // Print best solutions
    printf("Best solutions:\n");
    int sols_show = n_sols;
    if(sols_show>max_to_show) sols_show = max_to_show;
    for(int i=0;i<sols_show;i++){
        print_solution(stdout,sols[i],prob->multiplier);
    }
    printf("Saving solutions...\n");
    save_solutions(output_file_1,sols,sols_show,n_sols,
        prob->multiplier,
        input_file,n_random,pool_size,vision_range,
        seconds,max_size_found,elapsed_seconds);
    // Perform local search
    printf("Starting local searchs...\n");
    local_search_solutions(prob,sols,&n_sols);
    // Print best solutions
    printf("Best solutions after HC:\n");
    sols_show = n_sols;
    if(sols_show>max_to_show) sols_show = max_to_show;
    for(int i=0;i<sols_show;i++){
        print_solution(stdout,sols[i],prob->multiplier);
    }
    // ---@> Update timers
    end = clock();
    gettimeofday(&elapsed_end,NULL);
    seconds = (float)(end - start) / (float)CLOCKS_PER_SEC;
    elapsed_seconds = get_delta_seconds(elapsed_start,elapsed_end);
    //
    printf("All done in %f [s]!\n",seconds);
    printf("Saving solutions...\n");
    save_solutions(output_file_2,sols,sols_show,n_sols,
        prob->multiplier,
        input_file,n_random,pool_size,vision_range,
        seconds,max_size_found,elapsed_seconds);
    // Free memory
    for(int i=0;i<n_sols;i++){
        free(sols[i]);
    }
    free(sols);
    free(prob);

    return 0;
}
