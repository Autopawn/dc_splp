#include "load.h"

problem *load_simple_format(FILE *fp){
    // Alloc memory for problem.
    problem *prob = safe_malloc(sizeof(problem));

    // Read filename in header:
    char buffer[400];
    if(fscanf(fp,"FILE: %s",buffer)!=1){
        fprintf(stderr,"ERROR: couldn't read FILE!\n");
        exit(1);
    }
    printf("FILE: \"%s\"\n",buffer);

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

    // Third argument is the size restriction.
    int trd_num;
    if(fscanf(fp,"%d",&trd_num)!=1){
        fprintf(stderr,"ERROR: size restriction expected!\n");
        exit(1);
    }
    if(trd_num==0) trd_num=-1;
    prob->size_restriction = trd_num;
    printf("Size restriction: %d\n",prob->size_restriction);

    // For each facility
    for(int i=0;i<prob->n_facilities;i++){

        // Read facility index
        int facility_index;
        if(fscanf(fp,"%d",&facility_index)!=1){
            fprintf(stderr,"ERROR: facility index expected!\n");
        }
        assert(i+1==facility_index);

        // Read facility cost
        if(fscanf(fp,"%lld",&prob->facility_cost[i])!=1){
            fprintf(stderr,"ERROR: facility %d cost expected!\n",i);
            exit(1);
        }

        // Read each distance
        for(int j=0;j<prob->n_clients;j++){
            if(fscanf(fp,"%lld",&prob->distances[i][j])!=1){
                fprintf(stderr,"ERROR: distance from facility %d to client %d expected!\n",i,j);
                exit(1);
            }
        }
    }

    // Unsetted values:
    prob->multiplier = 1;
    prob->transport_cost = 1;
    for(int j=0;j<prob->n_clients;j++){
        prob->weights[j] = 1;
    }

    // Compute facility distance matrix:
    printf("Computing facility-facility distance matrix...\n");
    problem_create_facility_dist_matrix(prob);
    problem_create_facility_nearest_matrix(prob);

    //
    return prob;
}

problem *load_orlib_format(FILE *fp){
    // Alloc memory for problem.
    problem *prob = safe_malloc(sizeof(problem));

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

    // For each facility
    int cap_0_warn = 0;
    for(int i=0;i<prob->n_facilities;i++){

        // Read facility capacity
        double capacity=0;
        char cap_text[200];
        if(fscanf(fp,"%lf",&capacity)!=1){
            if(fscanf(fp,"%s",cap_text)!=1){
                fprintf(stderr,"ERROR: facility capacity expected!\n");
                exit(1);
            }else{
                if(strcmp(cap_text,"capacity")!=0){
                    fprintf(stderr,"ERROR: facility capacity isn't valid!\n");
                    exit(1);
                }
            }
        }
        if(capacity!=0 && !cap_0_warn){
            fprintf(stderr,"WARNING: facility capacity is %lf (not 0)!\n",capacity);
            cap_0_warn = 1;
        }

        // Read facility cost
        double facility_cost;
        if(fscanf(fp,"%lf",&facility_cost)!=1){
            fprintf(stderr,"ERROR: facility %d cost expected!\n",i);
            exit(1);
        }
        prob->facility_cost[i] = (lint)(ORLIB_FORMAT_COST_MULT*facility_cost+0.499999999);
    }

    // For each client
    int all_demands_0 = 1;
    for(int j=0;j<prob->n_clients;j++){
        // Read client demand
        double demand;
        if(fscanf(fp,"%lf",&demand)!=1){
            fprintf(stderr,"ERROR: client %d demand expected!\n",j);
        }
        if(demand!=0) all_demands_0 = 0;
        prob->weights[j] = (lint) demand;

        // Add distances to facility-city matrix:
        for(int i=0;i<prob->n_facilities;i++){
            double dist;
            if(fscanf(fp,"%lf",&dist)!=1){
                fprintf(stderr,"ERROR: cost from facility %d to client %d expected!\n",i,j);
                exit(1);
            }
            if(demand==0){
                assert(all_demands_0 || dist==0);
                prob->distances[i][j] = (lint) (ORLIB_FORMAT_COST_MULT*dist+0.499999999);
            }else{
                prob->distances[i][j] = (lint) (ORLIB_FORMAT_COST_MULT*dist/demand+0.499999999);
            }
        }
    }

    // Unsetted values:
    prob->multiplier = ORLIB_FORMAT_COST_MULT;
    prob->size_restriction = -1; // SPLP
    prob->transport_cost = 1;
    if(all_demands_0){
        for(int j=0;j<prob->n_clients;j++){
            prob->weights[j] = 1;
        }
    }

    // Compute facility distance matrix:
    printf("Computing facility-facility distance matrix...\n");
    problem_create_facility_dist_matrix(prob);
    problem_create_facility_nearest_matrix(prob);

    //
    return prob;
}

problem *new_problem_load(const char *file){
    printf("Reading file \"%s\"...\n",file);
    FILE *fp = fopen(file,"r");
    if(fp==NULL){
        fprintf(stderr,"ERROR: couldn't open file \"%s\"!\n",file);
        exit(1);
    }
    // Read first string
    char buffer[400];
    if(fscanf(fp,"%s",buffer)!=1){
        fprintf(stderr,"ERROR: couldn't read first string!\n");
        exit(1);
    }
    int simple_format = (strcmp(buffer,"FILE:")==0);

    fseek(fp,0,SEEK_SET);
    problem *prob;
    if(simple_format){
        prob = load_simple_format(fp);
    }else{
        prob = load_orlib_format(fp);
    }

    // Close file
    fclose(fp);
    printf("Done reading.\n");

    return prob;
}

void save_solutions(const char *file, solution **sols, int n_sols, int tot_n_sols,
        double multiplier,
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
    #ifdef FDIST_SUM_MODE
    fprintf(fp,"# Facility_dist: SUM_OF_DELTAS\n");
    #else
    fprintf(fp,"# Facility_dist: MINIMUM\n");
    #endif
    #ifdef ONLY_BESTS
    fprintf(fp,"# Reduction_mode: ONLY_BESTS\n");
    #else
    fprintf(fp,"# Reduction_mode: DISPERSE\n");
    #endif
    // Print the solutions:
    if(n_sols==0){
        solution empty_sol = empty_solution();
        print_solution(fp,&empty_sol,multiplier);
    }else{
        for(int i=0;i<n_sols;i++){
            print_solution(fp,sols[i],multiplier);
        }
    }
    fclose(fp);
}

// printing functions:
void print_solution(FILE *f, const solution *sol, double multiplier){
    fprintf(f,"SOLUTION:\n");
    double value = sol->value/multiplier;
    fprintf(f,"  Value: %lf\n",value);
    fprintf(f,"  Assigns: ");
    for(int i=0;i<MAX_CLIENTS;i++){
        if(sol->assignments[i]!=-1){
            fprintf(f,"%d ",sol->assignments[i]);
        }
    }
    fprintf(f,"\n");
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
