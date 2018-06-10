#include "reduce.h"

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// DISSIMILITUDE PAIRS HEAP
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// NOTE: Is possible to replace lint=>float and int=>ushort to use half of the memory saving pair data.
typedef struct{
    lint dissim;
    int indx_a, indx_b;
} dissimpair;

dissimpair heap_poll(dissimpair *heap, int *size){
    dissimpair retp = heap[0];
    heap[0] = heap[*size-1];
    *size -= 1;
    // Heapify down:
    int i = 0;
    int c = 2*i+1;
    while(c<*size){
        if(c+1<*size && heap[c+1].dissim<heap[c].dissim) c = c+1;
        if(heap[i].dissim<heap[c].dissim) break;
        dissimpair aux = heap[i];
        heap[i] = heap[c];
        heap[c] = aux;
        i = c;
        c = 2*i+1;
    }
    //
    return retp;
}

void heap_add(dissimpair *heap, int *size, dissimpair val){
    heap[*size] = val;
    *size += 1;
    // Heapify up:
    int i = *size-1;
    int p = (i-1)/2;
    while(i>0 && heap[i].dissim<heap[p].dissim){
        dissimpair aux = heap[i];
        heap[i] = heap[p];
        heap[p] = aux;
        i = p;
        p = (i-1)/2;
    }
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// REDUCTION PROCESS
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// Reduces an array (sols) with pointers to a set of solutions to target_n size, freeing memory of the discarted ones. *n_sols is modified.
void reduce_solutions(const problem *prob,
        solution **sols, int *n_sols, int target_n, int vision_range){
    // If the vision range is -1, use random selection.
    if(vision_range==-1 && *n_sols>target_n){
        // Put target_n randomly selected solutions first on the array:
        for(int i=0;i<target_n;i++){
            int choice = i+rand()%(*n_sols-i);
            solution *aux = sols[i];
            sols[i] = sols[choice];
            sols[choice] = aux;
        }
        // Free other solutions:
        for(int i=target_n;i<*n_sols;i++){
            free(sols[i]);
        }
        // Set the amount of solutions right.
        *n_sols = target_n;
        return;
    }
    // Ensure that the vision_range isn't larger than the number of solutions.
    if(vision_range>*n_sols) vision_range = *n_sols;
    // Sort solution pointers from larger to smaller value of the solution.
    qsort(sols,*n_sols,sizeof(solution*),solution_value_cmp_inv);
    #ifdef VERBOSE
        printf("#BASESORTED\n");
        print_solsets(sols,*n_sols);
    #endif
    // Return if there is no need of reduction.
    if(*n_sols<=target_n) return;
    // Double linked structure to know solutions that haven't yet been discarted:
    int *discarted = safe_malloc((*n_sols)*sizeof(int));
    int *nexts = safe_malloc((*n_sols)*sizeof(int));
    int *prevs = safe_malloc((*n_sols)*sizeof(int));
    for(int i=0;i<*n_sols;i++){
        discarted[i] = 0;
        prevs[i] = i-1;
        nexts[i] = i+1;
    }
    prevs[0] = -1;
    nexts[*n_sols-1] = -1;
    // Heap of dissimilitude pairs
    int n_pairs = 0;
    dissimpair *heap = safe_malloc(sizeof(dissimpair)*2*(*n_sols)*vision_range);
    // Initial set of dissimilitude pairs
    for(int i=0;i<*n_sols;i++){
        for(int j=1;j<=vision_range;j++){
            if(i+j>=*n_sols) break;
            dissimpair pair;
            pair.indx_a = i;
            pair.indx_b = i+j;
            pair.dissim = solution_dissimilitude(prob,
                sols[pair.indx_a],sols[pair.indx_b]);
            #ifdef PAIR_DISTANCE
            printf("#INITIAL DISSIM %d(%lld) %d(%lld) %llu\n",
                pair.indx_a,sols[pair.indx_a]->value,
                pair.indx_b,sols[pair.indx_b]->value,
                pair.dissim);
            #endif
            heap_add(heap,&n_pairs,pair);
        }
    }
    // Eliminate as much solutions as required:
    int n_eliminate = *n_sols-target_n;
    int elims = 0;
    while(elims<n_eliminate){
        // Eliminate worst solution of most similar pair
        if(n_pairs==0) break;
        dissimpair pair = heap_poll(heap,&n_pairs);
        if(!discarted[pair.indx_a] && !discarted[pair.indx_b]){
            #ifdef PAIR_DISTANCE
            // Get the distance on the linked list of the solutions of the pair:
            // NOTE: this may be expensive to do.
            int idx = pair.indx_a;
            int llist_dist = 0;
            while(idx != pair.indx_b){
                idx = nexts[idx];
                assert(idx!=-1);
                llist_dist++;
            }
            // Print the indexes, dissimilitude and distance in the linked list:
            int n_base = sols[pair.indx_a]->n_facilities;
            printf("#DIST %d %d %d %llu %d\n",
                n_base,pair.indx_a,pair.indx_b,pair.dissim,llist_dist);
            #endif
            // Delete the second solution on the pair.
            int to_delete = pair.indx_b;
            discarted[to_delete] = 1;
            free(sols[to_delete]);
            elims += 1;
            // Update double linked list:
            if(nexts[to_delete]!=-1) prevs[nexts[to_delete]] = prevs[to_delete];
            if(prevs[to_delete]!=-1) nexts[prevs[to_delete]] = nexts[to_delete];
            // Add new pairs to replace those that will be deleted on the destroyed solution.
            int *prev_sols = safe_malloc(sizeof(int)*vision_range);
            int *next_sols = safe_malloc(sizeof(int)*vision_range);
            int iter;
            // Get solutions after
            iter = to_delete;
            for(int i=0;i<vision_range;i++){
                if(nexts[iter]==-1){
                    next_sols[i] = -1;
                }else{
                    iter = nexts[iter];
                    next_sols[i] = iter;
                }
            }
            // Get solutions before
            iter = to_delete;
            for(int i=0;i<vision_range;i++){
                if(prevs[iter]==-1){
                    prev_sols[i] = -1;
                }else{
                    iter = prevs[iter];
                    prev_sols[i] = iter;
                }
            }
            // Create new pairs
            for(int i=0;i<vision_range;i++){
                int pair_a = prev_sols[vision_range-1-i];
                int pair_b = next_sols[i];
                if(pair_a!=-1 && pair_b!=-1){
                    // Create the replace node:
                    dissimpair pair;
                    pair.indx_a = pair_a;
                    pair.indx_b = pair_b;
                    pair.dissim = solution_dissimilitude(prob,
                        sols[pair.indx_a],sols[pair.indx_b]);
                    #ifdef PAIR_DISTANCE
                    printf("#REPLACEMENT DISSIM %d(%lld) %d(%lld) %llu\n",
                        pair.indx_a,sols[pair.indx_a]->value,
                        pair.indx_b,sols[pair.indx_b]->value,
                        pair.dissim);
                    #endif
                    assert(n_pairs<2*(*n_sols)*vision_range);
                    heap_add(heap,&n_pairs,pair);
                }
            }
            //
            free(prev_sols);
            free(next_sols);
        }
    }
    // Free all the pairs:
    free(heap);
    n_pairs = 0;
    // Set output final array:
    int new_nsols=0;
    for(int i=0;i<*n_sols;i++){
        if(discarted[i]==0){
            sols[new_nsols] = sols[i];
            new_nsols += 1;
        }
    }
    *n_sols = new_nsols;
    // Free arrays
    free(discarted);
    free(nexts);
    free(prevs);
}
