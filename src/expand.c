#include "expand.h"

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// FUTURESOL
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// Possible future solution that results from another one.

typedef struct {
    solution *origin;
    int newf;
    uint hash;
    int n_facilities;
    short facilities[0]; // Flexible array member.
} futuresol;

int futuresol_cmp(const void *a, const void *b){
    const futuresol *aa = (const futuresol *) a;
    const futuresol *bb = (const futuresol *) b;
    if(aa->hash>bb->hash) return +1;
    if(aa->hash<bb->hash) return -1;
    int nf_delta = aa->n_facilities - bb->n_facilities;
    if(nf_delta!=0) return nf_delta;
    for(int i=0;i<aa->n_facilities;i++){
        int idx_delta = aa->facilities[i]-bb->facilities[i];
        if(idx_delta!=0) return idx_delta;
    }
    return 0;
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// EXPANSION PROCESS
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// | From n_sols solutions and an array to pointers to them (sols), create new solutions and return an array of pointers to them, also sets the out_n_sols value to the length of the created array.
solution **new_expand_solutions(const problem *prob,
        solution** sols, int n_sols, int *out_n_sols){
    int csize = sols[0]->n_facilities;
    size_t fsol_size = sizeof(futuresol)+sizeof(short)*(csize+1);
    void *futuresols = safe_malloc(fsol_size*n_sols*prob->n_facilities);
    int n_futuresols = 0;
    // Create solutions for the next iteration.
    for(int i=0;i<n_sols;i++){
        assert(sols[i]->n_facilities==csize);
        for(short f=0;f<prob->n_facilities;f++){
            futuresol *fsol = (futuresol *)(futuresols+fsol_size*n_futuresols);
            // Create a potential future solution, with the old one and adding a facility.
            fsol->origin = sols[i];
            fsol->newf = f;
            fsol->hash = hash_int(fsol->newf);
            fsol->n_facilities = csize;
            // Copy its facilities, and check if f already exists.
            int f_is_new=1;
            for(int k=0;k<csize;k++){
                if(sols[i]->facilities[k]==f){
                    f_is_new = 0;
                    break;
                }
                fsol->facilities[k] = sols[i]->facilities[k];
                fsol->hash = fsol->hash ^ hash_int(sols[i]->facilities[k]);
            }
            if(!f_is_new) continue;
            add_to_sorted(fsol->facilities,&fsol->n_facilities,f);
            n_futuresols += 1;
        }
    }
    // Sort futuresols to detect the ones that are the same faster.
    qsort(futuresols,n_futuresols,fsol_size,futuresol_cmp);
    int new_n_futuresols = 0;
    futuresol *last_fsol = NULL;
    for(int r=0;r<n_futuresols;r++){
        futuresol *fsol = (futuresol *)(futuresols+fsol_size*r);
        // Compare fsol with the last_fsol:
        int ftsol_cmp = 0;
        if(last_fsol!=NULL) ftsol_cmp = futuresol_cmp(last_fsol,fsol);
        // Check if fsol creates a brave new solution.
        if(last_fsol==NULL || ftsol_cmp!=0){
            futuresol *next_pos = (futuresol *)
                (futuresols+fsol_size*new_n_futuresols);
            memcpy(next_pos,fsol,fsol_size);
            last_fsol = next_pos;
            new_n_futuresols += 1;
        }
        /* Check if fsol doesn't create a new solution but creates it from a better one, in that case fsol replaces last_fsol. Because the new
        solution should be better that the better one that generates it */
        if(last_fsol!=NULL && ftsol_cmp==0){
            int is_better = fsol->origin->value>last_fsol->origin->value;
            if(is_better) memcpy(last_fsol,fsol,fsol_size);
        }
    }
    n_futuresols = new_n_futuresols;
    // Create the new solutions:
    solution **out_sols = safe_malloc(sizeof(solution*)*n_futuresols);
    *out_n_sols = 0;
    for(int r=0;r<n_futuresols;r++){
        futuresol *fsol = (futuresol *)(futuresols+fsol_size*r);
        solution *new_sol = safe_malloc(sizeof(solution));
        *new_sol = *fsol->origin;
        solution_add(prob,new_sol,fsol->newf);
        lint delta = new_sol->value - fsol->origin->value;
        #ifdef PRINT_EXPANSIONS
        printf("#EXPAND {");
            for(int k=0;k<new_sol->n_facilities;k++){
                printf("%d",new_sol->facilities[k]);
                if(k<new_sol->n_facilities-1) printf(",");
            }
            printf("} :%lld %lld\n",new_sol->value,delta);
        #endif
        if(delta<=0){
            free(new_sol);
            continue;
        }
        out_sols[*out_n_sols] = new_sol;
        *out_n_sols += 1;
    }
    free(futuresols);
    return out_sols;
}
