#include "dsa.h"

#include <math.h>
#include <time.h>

int main(int argc, char **argv){
    int pool_size,vision_range,max_to_show;
    const char *input_file = NULL;
    int good = 1;
    #ifdef LOCAL_SEARCH
        if(argc!=7) good = 0;
    #else
        if(argc!=6) good = 0;
    #endif
    if(good){
        if(sscanf(argv[1],"%d",&pool_size)!=1) good = 0;
        if(sscanf(argv[2],"%d",&vision_range)!=1) good = 0;
        if(sscanf(argv[3],"%d",&max_to_show)!=1) good = 0;
        input_file = argv[4];
    }
    if(!good){
        #ifdef LOCAL_SEARCH
            printf("Usage: %s <pool_size> <vision_range> <max_sols_to_show> <problem_file> <output_before_ls> <output_after_ls>\n",argv[0]);
        #else
            printf("Usage: %s <pool_size> <vision_range> <max_sols_to_show> <problem_file> <output_file>\n",argv[0]);
        #endif
        printf("A vision_range of -1 will activate random choice reduction.\n");
        printf("A pool_size of 1 will activate greedy mode.\n");
        exit(1);
    }
    printf("Pool size: %d\n",pool_size);
    if(vision_range!=-1){
        if(pool_size==1){
            vision_range=1;
            printf("Vision range: %d\n",vision_range);
            printf("\t(Greedy)\n");
        }else{
            printf("Vision range: %d\n",vision_range);
        }
    }else{
        printf("Vision range: %d\n",vision_range);
        printf("\t(Random selection)\n");
    }
    assert(vision_range>=-1);
    printf("Max solutions to show: %d\n",max_to_show);
    // Load problem file:
    problem *prob = new_problem_load(input_file);
    // Get the solutions:
    int n_sols, max_size_found;
    printf("Starting search...\n");
    clock_t start = clock();
    solution **sols = new_find_best_solutions(prob,
        pool_size, vision_range, &n_sols, &max_size_found);
    clock_t end = clock();
    float seconds = (float)(end - start) / (float)CLOCKS_PER_SEC;
    printf("Max size solution found: %d\n",max_size_found);
    printf("Search done in %f [s]!\n",seconds);
    // Print best solutions
    printf("Best solutions:\n");
    int sols_show = n_sols;
    if(sols_show>max_to_show) sols_show = max_to_show;
    for(int i=0;i<sols_show;i++){
        print_solution(stdout,sols[i]);
    }
    printf("Saving solutions...\n");
    save_solutions(argv[5],sols,sols_show,n_sols,input_file,pool_size,vision_range,
        seconds,max_size_found);
    // Perform local search
    #ifdef LOCAL_SEARCH
        printf("Starting local searchs...\n");
        local_search_solutions(prob,sols,&n_sols);
        // Print best solutions
        printf("Best solutions after HC:\n");
        sols_show = n_sols;
        if(sols_show>max_to_show) sols_show = max_to_show;
        for(int i=0;i<sols_show;i++){
            print_solution(stdout,sols[i]);
        }
        // Update timer
        end = clock();
        seconds = (float)(end - start) / (float)CLOCKS_PER_SEC;
        printf("All done in %f [s]!\n",seconds);
        printf("Saving solutions...\n");
        save_solutions(argv[6],sols,sols_show,n_sols,input_file,pool_size,vision_range,
            seconds,max_size_found);
    #endif
    // Free memory
    for(int i=0;i<n_sols;i++){
        free(sols[i]);
    }
    free(sols);
    free(prob);

    return 0;
}