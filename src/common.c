#include "common.h"

void *safe_malloc(size_t size){
    assert(errno==0);
    void *ptr = malloc(size);
    if((size>0 && ptr==NULL) || errno!=0){
        fprintf(stderr,"ERROR (on malloc): %s\n",strerror(errno));
        exit(1);
    }
    return ptr;
}

uint hash_int(uint x){
    // Thanks to https://stackoverflow.com/a/12996028
    x = ((x >> 16)^x)*0x45d9f3b;
    x = ((x >> 16)^x)*0x45d9f3b;
    x = (x >> 16)^x;
    return x;
}

void add_to_sorted(short *array, int *len, short val){
    int place = *len;
    while(place>0){
        if(array[place-1]<=val) break;
        array[place] = array[place-1];
        place--;
    }
    array[place] = val;
    *len += 1;
}

void rem_of_sorted(short *array, int *len, short val){
    int place=-1;
    for(int i=0;i<*len;i++){
        if(array[i]==val){
            place = i;
            break;
        }
    }
    assert(place!=-1); // Not in array.
    for(int i=place;i<*len-1;i++){
        array[i] = array[i+1];
    }
    *len -= 1;
}

// ----

typedef struct{
    lint dist;
    int indx;
} distduple;

int distduple_cmp(const void *a, const void *b){
    const distduple *aa = (const distduple *) a;
    const distduple *bb = (const distduple *) b;
    if(aa->dist==bb->dist) return 0;
    else if(aa->dist<bb->dist) return -1;
    else return +1;
}

void problem_precompute(problem *prob){
    #if defined(DISSIM_MSE) || defined(DISSIM_HAUSDORFF)
        // ---@>
        // sets the distance between facilities (the fdistance matrix)
        // mode 's':   d(a,b) = min_j d(a,j)+d(b,j)
        // mode 'm':   d(a,b) = sum_j |d(a,j)-d(b,j)|
        // it requires prob->distances setted.
        for(int a=0;a<prob->n_facilities;a++){
            for(int b=a;b<prob->n_facilities;b++){
                #ifdef FDISMODE_SUMOFDELTAS
                lint dist = 0;
                for(int j=0;j<prob->n_clients;j++){
                    lint delta = prob->distances[a][j]-prob->distances[b][j];
                    if(delta<0) delta = -delta;
                    dist += delta;
                }
                #endif
                #ifdef FDISMODE_MINDIST
                lint dist = MAX_LINT;
                for(int j=0;j<prob->n_clients;j++){
                    lint dist_sum = prob->distances[a][j]+prob->distances[b][j];
                    if(dist_sum<dist) dist = dist_sum;
                }
                #endif
                prob->fdistances[a][b] = dist;
                prob->fdistances[b][a] = dist;
            }
        }
        // ---@>
        #ifdef DISSIM_MSE
            // Set the matrix of nearest to farthest facilities for each one
            // requires prob->fdistances setted (above).
            distduple *duples = safe_malloc(sizeof(distduple)*prob->n_facilities);
            for(int i=0;i<prob->n_facilities;i++){
                for(int j=0;j<prob->n_facilities;j++){
                    duples[j].indx = j;
                    duples[j].dist = prob->fdistances[i][j];
                }
                qsort(duples,prob->n_facilities,sizeof(distduple),distduple_cmp);
                for(int j=0;j<prob->n_facilities;j++){
                    prob->fnearest[i][j] = duples[j].indx; //NOTE
                }
            }
            free(duples);
        #endif
    #endif
}
