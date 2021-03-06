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
    int diff=0;
    //
    lint ldiff = (*bb)->value - (*aa)->value; // NOTE: fn. can be used to sort by decreasing value.
    if(ldiff!=0){
        if(ldiff>0) diff=1;
        if(ldiff<0) diff=-1;
        return diff;
    }
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
    // Reassign clients to the new facility
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
    for(int i=0;i<sol->n_facilities;i++){
        nvalue -= prob->facility_cost[sol->facilities[i]];
    }
    // Update solution value:
    sol->value = nvalue;
}

// | Removes a facility of the solution:
void solution_remove(const problem *prob, solution *sol, short remf){
    #ifndef NDEBUG
        // Assert that remf is on the solution
        int inner_i = -1;
        for(int f=0;f<sol->n_facilities;f++){
            if(sol->facilities[f]==remf){
                inner_i=f;
                break;
            }
        }
        assert(inner_i!=-1);
    #endif
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
    for(int i=0;i<sol->n_facilities;i++){
        nvalue -= prob->facility_cost[sol->facilities[i]];
    }
    // Update solution value:
    sol->value = nvalue;
}


#ifdef DISSIM_CLIENTDELTA

// Returns the dissimilitude (using client delta).
lint solution_dissimilitude(const problem *prob, const solution *sol_a, const solution *sol_b){
    lint total = 0;
    for(int i=0;i<prob->n_clients;i++){
        lint cost_a = prob->distances[sol_a->assignments[i]][i];
        lint cost_b = prob->distances[sol_b->assignments[i]][i];
        lint delta = cost_a-cost_b;
        if(delta<0) delta = -delta;
        total += delta;
    }
    return total;
}

#endif

#ifdef DISSIM_HAUSDORFF

// Returns the dissimilitude (using Hausdorff):
lint solution_dissimilitude(const problem *prob, const solution *sol_a, const solution *sol_b){
    lint disim = 0;
    for(int t=0;t<2;t++){
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
        // Swap solutions for 2nd iteration:
        const solution *aux = sol_a;
        sol_a = sol_b;
        sol_b = aux;
    }
    return disim;
}

#endif

#ifdef DISSIM_MSE

// Returns the dissimilitude (using mean geometric error).
lint solution_dissimilitude(const problem *prob, const solution *sol_a, const solution *sol_b){
    lint disim = 0;
    for(int t=0;t<2;t++){
        // Check the mode that will be used
        // modeA compares each facility with the others
        // modeB checks for each facility from the nearest ot the farthest
        if(sol_b->n_facilities*sol_b->n_facilities<prob->n_facilities){
            // MODE A
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
        }else{
            // MODE B
            // Precompute array of presences of the solution b
            char *presence = safe_malloc(sizeof(char)*prob->n_facilities);
            memset(presence,0,sizeof(char)*prob->n_facilities);
            for(int bi=0;bi<sol_b->n_facilities;bi++){
                short f_b = sol_b->facilities[bi];
                presence[f_b] = 1;
            }
            // Add distance from each facility in A to B.
            for(int ai=0;ai<sol_a->n_facilities;ai++){
                short f_a = sol_a->facilities[ai];
                // Find the nearest presence
                short f_b = -1;
                for(int k=0;k<prob->n_facilities;k++){
                    f_b = prob->fnearest[f_a][k];
                    if(presence[f_b]) break;
                }
                assert(f_b!=-1);
                // Add that distance
                disim += prob->fdistances[f_a][f_b];
            }
            //
            free(presence);
        }
        // Swap solutions for 2nd iteration:
        const solution *aux = sol_a;
        sol_a = sol_b;
        sol_b = aux;
    }
    return disim;
}

#endif

void solution_copy(const problem *prob, solution *dest, const solution *source){
    // Copies a solution to a destiny
    dest->n_facilities = source->n_facilities;
    for(int i=0;i<dest->n_facilities;i++){
        dest->facilities[i] = source->facilities[i];
    }
    for(int j=0;j<prob->n_clients;j++){
        dest->assignments[j] = source->assignments[j];
    }
    if(prob->n_clients<MAX_CLIENTS){
        assert(source->assignments[prob->n_clients]==-1);
        dest->assignments[prob->n_clients] = -1;
    }
    dest->value = source->value;
}

// Takes a solution and uses hill climbing with best-improvement, using an exchange movement.
solution solution_hill_climbing(const problem *prob, solution sol){
    #ifndef NDEBUG
        lint old_value = sol.value;
        int old_n_facilities = sol.n_facilities;
    #endif
    if(sol.n_facilities==0) return sol;
    solution best = sol;
    #ifndef LOCALSEARCH_DONT_USE_WHITAKER
        if(sol.n_facilities>=2){
            best = solution_whitaker_hill_climbing(prob,sol);
        }else
    #else
        {
        // This is the old hill climbing, I will only use it for solutions of size 1, which should be trivial.
        int improvement = 1;
        while(improvement){
            improvement = 0;
            // Remove a facility:
            solution cand0; solution_copy(prob,&cand0,&best);
            for(int k=0;k<sol.n_facilities;k++){
                assert(cand0.n_facilities==sol.n_facilities);
                solution cand1; solution_copy(prob,&cand1,&cand0);
                solution_remove(prob,&cand1,cand0.facilities[k]);
                // Add facility j:
                for(int j=0;j<prob->n_facilities;j++){
                    solution cand2; solution_copy(prob,&cand2,&cand1);
                    solution_add(prob,&cand2,j);
                    if(cand2.n_facilities==cand0.n_facilities &&
                            cand2.value>best.value){
                        solution_copy(prob,&best,&cand2);
                        improvement = 1;
                    }
                }
            }
        }
        }
    #endif
    assert(best.value>=old_value);
    assert(best.n_facilities==old_n_facilities);
    return best;
}

void solution_client_2_nearest(const problem *prob, const solution *sol,
        int cli, int *phi1, int *phi2){
    // Nearest facility
    *phi1 = -1;
    for(int p=0;p<sol->n_facilities;p++){
        int fac = sol->facilities[p];
        if(*phi1==-1 || prob->distances[*phi1][cli]>prob->distances[fac][cli]){
            *phi1 = fac;
        }
    }
    // 2nd nearest
    *phi2 = -1;
    for(int p=0;p<sol->n_facilities;p++){
        int fac = sol->facilities[p];
        if(*phi1==fac) continue;
        if(*phi2==-1 || prob->distances[*phi2][cli]>prob->distances[fac][cli]){
            *phi2 = fac;
        }
    }
}

void solution_findout(const problem *prob, const solution *sol, int f_ins, lint *v,
        const int *phi1, const int *phi2, int *out_f_rem, lint *out_profit){
    // NOTE: v must be intialized with LINT_MIN and have size equal to prob->n_facilities.
    // ^ It is always reset to that state after computation.
    lint w = -prob->facility_cost[f_ins];
    for(int k=0;k<sol->n_facilities;k++){
        v[sol->facilities[k]] = -prob->facility_cost[sol->facilities[k]];
    }
    //
    for(int u=0;u<prob->n_clients;u++){
        lint delta = prob->distances[phi1[u]][u]-prob->distances[f_ins][u];
        if(delta>=0){ // Profit by adding f_ins, because is nearly.
            w += prob->weights[u]*delta;
        }else{ // Loss by removing phi[u], because it is nearly.
            if(v[phi1[u]]==MIN_LINT) continue; // phi1[u] not part of the solution.
            if(prob->distances[f_ins][u]<prob->distances[phi2[u]][u]){
                v[phi1[u]] += prob->weights[u]*(prob->distances[f_ins][u]-prob->distances[phi1[u]][u]);
            }else{
                v[phi1[u]] += prob->weights[u]*(prob->distances[phi2[u]][u]-prob->distances[phi1[u]][u]);
            }
        }
    }
    //
    int f_rem = sol->facilities[0];
    for(int k=1;k<sol->n_facilities;k++){
        if(v[sol->facilities[k]]<v[f_rem]) f_rem = sol->facilities[k];
    }
    *out_f_rem = f_rem;

    *out_profit = w - v[*out_f_rem];
    // Reset v
    for(int k=0;k<sol->n_facilities;k++){
        v[sol->facilities[k]] = MIN_LINT;
    }
}


solution solution_whitaker_hill_climbing(const problem *prob, solution sol){
    assert(sol.n_facilities>=2);
    // -1 initialized array for solution_findout:
    lint v[MAX_FACILITIES];
    for(int i=0;i<prob->n_facilities;i++) v[i] = MIN_LINT;
    // Nearest facilities
    int phi1[MAX_CLIENTS];
    int phi2[MAX_CLIENTS];
    //
    while(1){
        // Array if facility appears in the solution
        int used[MAX_FACILITIES];
        for(int i=0;i<prob->n_facilities;i++) used[i] = 0;
        for(int k=0;k<sol.n_facilities;k++) used[sol.facilities[k]] = 1;
        // Clients nearest to the solution
        for(int i=0;i<prob->n_clients;i++){
            solution_client_2_nearest(prob,&sol,i,&phi1[i],&phi2[i]);
        }
        // Insertion candidate:
        lint best_delta = 0;
        int best_rem = -1;
        int best_ins = -1;
        for(int f=0;f<prob->n_facilities;f++){
            if(used[f]) continue;
            // Find the best option for removal:
            int f_rem;
            lint delta_profit;
            solution_findout(prob,&sol,f,v,phi1,phi2,&f_rem,&delta_profit);
            if(delta_profit>best_delta){
                best_delta = delta_profit;
                best_rem = f_rem;
                best_ins = f;
            }
        }
        // Stop when done:
        if(best_ins==-1) break;
        // Perform swap:
        #ifndef NDEBUG
            lint old_value = sol.value;
        #endif
        solution_remove(prob,&sol,best_rem);
        solution_add(prob,&sol,best_ins);
        assert(sol.value>old_value);
    }
    return sol;
}

solution random_solution(const problem *prob, int size){
    solution sol = empty_solution();
    while(sol.n_facilities<size){
        int newf = rand()%prob->n_facilities;
        solution_add(prob,&sol,newf);
    }
    return sol;
}
