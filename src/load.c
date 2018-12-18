#include "load.h"

problem *new_problem_load(const char *file){
    FILE *fp;
    printf("Reading file \"%s\"...\n",file);
    fp = fopen(file,"r");
    if(fp==NULL){
        fprintf(stderr,"ERROR: couldn't open file \"%s\"!\n",file);
        exit(1);
    }
    // Alloc memory for problem.
    problem *prob = malloc(sizeof(problem));

    // Read the facility cost:
    lint fcost;
    if(fscanf(fp,"%lld",&fcost)!=1){
        fprintf(stderr,"ERROR: facility cost expected!\n");
        exit(1);
    }
    if(fcost<0){
        prob->facility_fixed_cost = 0;
        prob->size_restriction = -fcost;
        printf("--- p-median PROBLEM ---\n");
        printf("Facilities: %d\n",prob->size_restriction);
        assert(prob->size_restriction<=MAX_SOL_SIZE);
    }else{
        prob->facility_fixed_cost = fcost;
        prob->size_restriction = -1;
        printf("--- SPLP PROBLEM ---\n");
        printf("Facility cost: %lld\n",prob->facility_fixed_cost);
    }

    // Read the transport cost:
    if(fscanf(fp,"%lld",&prob->transport_cost)!=1){
        fprintf(stderr,"ERROR: transport cost expected!\n");
        exit(1);
    }
    printf("Transport cost: %lld\n",prob->transport_cost);

    // Read the number of facilities:
    if(fscanf(fp,"%d",&prob->n_facilities)!=1){
        fprintf(stderr,"ERROR: number of facilities expected!\n");
        exit(1);
    }
    printf("N Facilities: %d\n",prob->n_facilities);
    assert(prob->n_facilities<=MAX_FACILITIES);

    // Read the number of clients:
    if(fscanf(fp,"%d",&prob->n_clients)!=1){
        fprintf(stderr,"ERROR: number of clients expected!\n");
        exit(1);
    }
    printf("N Clients: %d\n",prob->n_clients);
    assert(prob->n_clients<=MAX_CLIENTS);

    // Read the facility positions
    printf("Reading facility positions...\n");
    lint fxs[MAX_FACILITIES];
    lint fys[MAX_FACILITIES];
    for(int i=0;i<prob->n_facilities;i++){
        int result = fscanf(fp,"%lld %lld",&fxs[i],&fys[i]);
        if(result==EOF){
            fprintf(stderr,"ERROR: EOF while reading facility positions!\n");
            exit(1);
        }else if(result!=2){
            fprintf(stderr,"ERROR: Positions expected!\n");
            exit(1);
        }
    }

    // Read the client positions
    printf("Reading client positions...\n");
    lint cxs[MAX_FACILITIES];
    lint cys[MAX_FACILITIES];
    for(int i=0;i<prob->n_facilities;i++){
        int result = fscanf(fp,"%lld %lld",&cxs[i],&cys[i]);
        if(result==EOF){
            fprintf(stderr,"ERROR: EOF while reading client positions!\n");
            exit(1);
        }else if(result!=2){
            fprintf(stderr,"ERROR: Positions expected!\n");
            exit(1);
        }
    }

    // Read clients weights:
    int minw=-1,maxw=-1;
    printf("Reading client weights...\n");
    for(int i=0;i<prob->n_clients;i++){
        int result = fscanf(fp,"%d",&prob->weights[i]);
        if(result==EOF){
            fprintf(stderr,"ERROR: EOF while reading client weight!\n");
            exit(1);
        }else if(result!=1){
            fprintf(stderr,"ERROR: Weight expected!\n");
            exit(1);
        }
        if(i==0 || prob->weights[i]<minw) minw = prob->weights[i];
        if(i==0 || prob->weights[i]>maxw) maxw = prob->weights[i];
    }
    printf("Min weight: %d\n",minw);
    printf("Max weight: %d\n",maxw);

    // Compute facility-facility distance matrix:
    printf("Computing facility-facility distance matrix...\n");
    for(int i=0;i<prob->n_facilities;i++){
        for(int j=0;j<prob->n_facilities;j++){
            lint delta_x = fxs[i]-fxs[j];
            lint delta_y = fys[i]-fys[j];
            lint dist = (lint) round(sqrt(delta_x*delta_x+delta_y*delta_y));
            prob->fdistances[i][j] = dist;
        }
    }

    // Compute facility-client distance matrix:
    printf("Computing facility-client distance matrix...\n");
    for(int i=0;i<prob->n_facilities;i++){
        for(int j=0;j<prob->n_clients;j++){
            lint delta_x = fxs[i]-cxs[j];
            lint delta_y = fys[i]-cys[j];
            lint dist = (lint) round(sqrt(delta_x*delta_x+delta_y*delta_y));
            prob->distances[i][j] = dist;
        }
    }
    //
    fclose(fp);
    printf("Done reading.\n");
    return prob;
}

void save_solutions(const char *file, solution **sols, int n_sols, int tot_n_sols,
        const char *input_file, int pool_size, int vision_range,
        float seconds, int n_iterations, float elapsed){
    FILE *fp;
    printf("Opening file \"%s\"...\n",file);
    fp = fopen(file,"w");
    if(fp==NULL){
        fprintf(stderr,"ERROR: couldn't open file \"%s\"!\n",file);
        exit(1);
    }
    // Print some aditional info:
    fprintf(fp,"# Time: %f\n",seconds);
    fprintf(fp,"# Elapsed: %f\n",elapsed);
    fprintf(fp,"# Input_file: %s\n",input_file);
    fprintf(fp,"# Pool_size: %d\n",pool_size);
    fprintf(fp,"# Iterations: %d\n",n_iterations);
    fprintf(fp,"# Final_solutions: %d\n",tot_n_sols);
    fprintf(fp,"# Vision_range: %d\n",vision_range);
    #ifdef HAUSDORFF
    fprintf(fp,"# Dissimilitude: HAUSDORFF\n");
    #else
    fprintf(fp,"# Dissimilitude: MEAN_GEOMETRIC_ERROR\n");
    #endif
    // Print the solutions:
    if(n_sols==0){
        solution empty_sol = empty_solution();
        print_solution(fp,&empty_sol);
    }else{
        for(int i=0;i<n_sols;i++){
            print_solution(fp,sols[i]);
        }
    }
    fclose(fp);
}

// printing functions:
void print_solution(FILE *f, const solution *sol){
    fprintf(f,"SOLUTION:\n");
    fprintf(f,"  Value: %lld\n",sol->value);
    fprintf(f,"  Facilities: %d\n",sol->n_facilities);
    for(int i=0;i<sol->n_facilities;i++){
        fprintf(f,"  %4d :",sol->facilities[i]);
        for(int j=0;j<MAX_CLIENTS;j++){
            if(sol->assignments[j]==sol->facilities[i]){
                fprintf(f,"%4d",j);
            }
        }
        fprintf(f,"\n");
    }
}

void print_solsets(solution **sols, int n_sols){
    printf("{");
    for(int i=0;i<n_sols;i++){
        printf("{");
        for(int k=0;k<sols[i]->n_facilities;k++){
            printf("%d",sols[i]->facilities[k]);
            if(k<sols[i]->n_facilities-1)printf(",");
        }
        printf("}");
        if(i<n_sols-1)printf(",");
    }
    printf("}\n");
}
