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
    if(fscanf(fp,"%lld",&prob->facility_fixed_cost)!=1){
        fprintf(stderr,"ERROR: facility cost expected!\n");
        exit(1);
    }
    printf("Facility cost: %lld\n",prob->facility_fixed_cost);

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

    // Read the facility distance matrix:
    int warning_nonzero_issued = 0;
    int warning_nonsymm_issued = 0;
    printf("Reading facility distance matrix...\n");
    for(int i=0;i<prob->n_facilities;i++){
        for(int j=0;j<prob->n_facilities;j++){
            int result = fscanf(fp,"%lld",&prob->fdistances[i][j]);
            if(result==EOF){
                fprintf(stderr,"ERROR: EOF while reading facility distance matrix!\n");
                exit(1);
            }else if(result!=1){
                fprintf(stderr,"ERROR: Distance expected!\n");
                exit(1);
            }
            if(i==j && prob->fdistances[i][j]!=0 &&
                    warning_nonzero_issued==0){
                printf("WARNING: Non-zero distance on diagonal.\n");
                warning_nonzero_issued = 1;
            }
            if(i>j && prob->fdistances[i][j]!=prob->fdistances[j][i] &&
                    warning_nonsymm_issued==0){
                printf("WARNING: Matrix is nonsymmetric.\n");
                warning_nonsymm_issued = 1;
            }
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

    // Read the facility-client distance matrix:
    printf("Reading facility-client distance matrix...\n");
    for(int i=0;i<prob->n_facilities;i++){
        for(int j=0;j<prob->n_clients;j++){
            int result = fscanf(fp,"%lld",&prob->distances[i][j]);
            if(result==EOF){
                fprintf(stderr,"ERROR: EOF while reading facility-client distance matrix!\n");
                exit(1);
            }else if(result!=1){
                fprintf(stderr,"ERROR: Distance expected!\n");
                exit(1);
            }
        }
    }
    //
    fclose(fp);
    printf("Done reading.\n");
    return prob;
}

void save_solutions(const char *file, solution **sols, int n_sols,
        const char *input_file, int pool_size, int vision_range,
        float seconds, int n_iterations){
    FILE *fp;
    printf("Opening file \"%s\"...\n",file);
    fp = fopen(file,"w");
    if(fp==NULL){
        fprintf(stderr,"ERROR: couldn't open file \"%s\"!\n",file);
        exit(1);
    }
    // Print some aditional info:
    fprintf(fp,"# Time: %f\n",seconds);
    fprintf(fp,"# Input_file: %s\n",input_file);
    fprintf(fp,"# Pool_size: %d\n",pool_size);
    fprintf(fp,"# Iterations: %d\n",n_iterations);
    fprintf(fp,"# Vision_range: %d\n",vision_range);
    #ifdef HAUSDORFF
    fprintf(fp,"# Dissimilitude: HAUSDORFF\n");
    #else
    fprintf(fp,"# Dissimilitude: MEAN_GEOMETRIC_ERROR\n");
    #endif
    // Print the solutions:
    for(int i=0;i<n_sols;i++){
        print_solution(fp,sols[i]);
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
