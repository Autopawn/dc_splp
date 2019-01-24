#include "common.h"

void *safe_malloc(size_t size){
    void *ptr = malloc(size);
    if(size>0 && ptr==NULL){
        fprintf(stderr,"ERROR: Not enough memory!\n");
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

void problem_create_facility_dist_matrix(problem *prob){
    // sets the distance between facilities: d(a,b) = min_j d(a,j)+d(b,j)
    // it requires prob->distances setted.
    for(int a=0;a<prob->n_facilities;a++){
        for(int b=a;b<prob->n_facilities;b++){
            lint min_dist = MAX_LINT;
            for(int j=0;j<prob->n_clients;j++){
                lint dist_sum = prob->distances[a][j]+prob->distances[b][j];
                if(dist_sum<min_dist) min_dist = dist_sum;
            }
            prob->fdistances[a][b] = min_dist;
            prob->fdistances[b][a] = min_dist;
        }
    }
}

typedef struct{
    lint dist;
    int indx;
} distduple;

int distduple_comp(const void *a, const void *b){
    const distduple *aa = (const distduple *) a;
    const distduple *bb = (const distduple *) b;
    return aa->dist-bb->dist;
}

void problem_create_facility_nearest_matrix(problem *prob){
    // sets the matrix of nearest to farthest facilities for each one
    // it requires prob->fdistances setted.
    distduple *duples = safe_malloc(sizeof(distduple)*prob->n_facilities);
    for(int i=0;i<prob->n_facilities;i++){
        for(int j=0;j<prob->n_facilities;j++){
            duples[j].indx = j;
            duples[j].dist = prob->fdistances[i][j];
        }
        qsort(duples,prob->n_facilities,sizeof(distduple),distduple_comp);
        for(int j=0;j<prob->n_facilities;j++){
            prob->fnearest[i][j] = duples[j].indx; //NOTE
        }
    }
    free(duples);
}
