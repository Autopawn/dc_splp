#include "solution.h"

// | Returns an empty solution, with no facilities.
solution empty_solution(){
    solution sol;
    sol.n_facilities = 0;
    for(int c=0;c<MAX_CLIENTS;c++) sol.assignments[c] = -1;
    sol.value = MIN_LINT/2;
    return sol;
}

// Compare solutions to sort on decreasing value
int solution_value_cmp_inv(const void *a, const void *b){
    solution **aa = (solution **) a;
    solution **bb = (solution **) b;
    lint diff = (*bb)->value-(*aa)->value;
    return diff==0 ? 0 : ((diff>0)? +1 : -1);
}

// Compare solutions for equality
int solution_cmp(const void *a, const void *b){
    const solution **aa = (const solution **) a;
    const solution **bb = (const solution **) b;
    int diff;
    //
    diff = (*bb)->value - (*aa)->value; // NOTE: fn. can be used to sort by decreasing value.
    if(diff!=0) return diff;
    //
    diff = (*bb)->n_facilities - (*aa)->n_facilities;
    if(diff!=0) return diff;
    //
    for(int i=0;i<(*aa)->n_facilities;i++){
        diff = (*bb)->facilities[i] - (*aa)->facilities[i];
        if(diff!=0) return diff;
    }
    return 0;
}

// | Adds a facility to the solution
void solution_add(const problem *prob, solution *sol, short newf){
    // Check if f is already on the solution:
    for(int f=0;f<sol->n_facilities;f++){
        if(sol->facilities[f]==newf) return;
    }
    // Add the facility to the solution.
    add_to_sorted(sol->facilities,&sol->n_facilities,newf);
    // | New value after adding the new facility.
    lint nvalue = 0;
    // Reassign clients to the new facility, from nearest to further.
    for(int c=0;c<prob->n_clients;c++){
        // Distance of that client to the new facility:
        lint distance = prob->distances[newf][c];
        // Distance to the previously assignated facility:
        lint old_distance = -1;
        if(sol->assignments[c]!=-1){
            old_distance = prob->distances[sol->assignments[c]][c];
        }
        if(old_distance==-1 || distance<old_distance){
            // ^ If client not assigned, or is nearest to the new facility assign it.
            // Cost of the new assignment:
            nvalue -= prob->weights[c]*prob->transport_cost*distance;
            // Reassign client to new facility
            sol->assignments[c] = newf;
        }else{
            // Cost of the previous assignment:
            nvalue -= prob->weights[c]*prob->transport_cost*old_distance;
        }
    }
    // The facility costs:
    nvalue -= prob->facility_fixed_cost*sol->n_facilities;
    // Update solution value:
    sol->value = nvalue;
}

// | Removes a facility of the solution:
void solution_remove(const problem *prob, solution *sol, short remf){
    // Assert that remf is on the solution
    int inner_i = -1;
    for(int f=0;f<sol->n_facilities;f++){
        if(sol->facilities[f]==remf){
            inner_i=f;
            break;
        }
    }
    assert(inner_i!=-1);
    // Remove the facility of the solution.
    rem_of_sorted(sol->facilities,&sol->n_facilities,remf);
    // The void solution:
    if(sol->n_facilities==0){
        *sol = empty_solution();
        return;
    }
    // | New value after after adding the new facility.
    lint nvalue = 0;
    // Drop clients of the facility.
    for(int c=0;c<prob->n_clients;c++){
        // If the client was owned by the facility,
        // reassign it to another facility of the solution:
        if(sol->assignments[c]==remf){
            short reaf = -1; // Final reassigned facility
            for(int i=0;i<sol->n_facilities;i++){
                short candf = sol->facilities[i];
                lint cand_distance = prob->distances[candf][c];
                if(reaf==-1 || cand_distance<prob->distances[reaf][c]){
                    reaf = candf;
                }
            }
            assert(reaf!=-1);
            // Reassign client
            sol->assignments[c] = reaf;
            // Cost of the new assignment:
            nvalue -= prob->weights[c]*prob->transport_cost*prob->distances[reaf][c];
        }else{
            // Cost of the previous assignment:
            lint old_distance = prob->distances[sol->assignments[c]][c];
            nvalue -= prob->weights[c]*prob->transport_cost*old_distance;
        }
    }
    // The facility costs:
    nvalue -= prob->facility_fixed_cost*sol->n_facilities;
    // Update solution value:
    sol->value = nvalue;
}

// Returns the dissimilitude (using mean geometric error or Hausdorff).
lint solution_dissimilitude(const problem *prob,
        const solution *sol_a, const solution *sol_b){
    lint disim = 0;
    for(int t=0;t<2;t++){
        #ifdef HAUSDORFF
            for(int ai=0;ai<sol_a->n_facilities;ai++){
                short f_a = sol_a->facilities[ai];
                lint cmin = MAX_LINT;
                for(int bi=0;bi<sol_b->n_facilities;bi++){
                    short f_b = sol_b->facilities[bi];
                    lint dist = prob->fdistances[f_a][f_b];
                    if(dist<cmin) cmin = dist;
                    if(cmin<disim) break;
                }
                if(disim<cmin && cmin<MAX_LINT) disim = cmin;
            }
        #else
            // Add distance from each facility in A to B.
            for(int ai=0;ai<sol_a->n_facilities;ai++){
                lint min_dist = -1;
                short f_a = sol_a->facilities[ai];
                for(int bi=0;bi<sol_b->n_facilities;bi++){
                    short f_b = sol_b->facilities[bi];
                    lint dist = prob->fdistances[f_a][f_b];
                    if(min_dist==-1 || dist<min_dist) min_dist = dist;
                }
                disim += min_dist;
            }
        #endif
        // Swap solutions for 2nd iteration:
        const solution *aux = sol_a;
        sol_a = sol_b;
        sol_b = aux;
    }
    return disim;
}

// Takes a solution and uses hill climbing with best-improvement, using an exchange movement.
solution solution_hill_climbing(const problem *prob, solution sol){
    if(sol.n_facilities==0) return sol;
    solution best = sol;
    //
    int improvement = 1;
    while(improvement){
        improvement = 0;
        // Remove a facility:
        solution cand0 = best;
        for(int k=0;k<sol.n_facilities;k++){
            assert(cand0.n_facilities==sol.n_facilities);
            solution cand1 = cand0;
            solution_remove(prob,&cand1,cand0.facilities[k]);
            // Add facility j:
            for(int j=0;j<prob->n_facilities;j++){
                solution cand2 = cand1;
                solution_add(prob,&cand2,j);
                if(cand2.n_facilities==cand0.n_facilities &&
                        cand2.value>best.value){
                    best = cand2;
                    improvement = 1;
                }
            }
        }
    }
    return best;
}
