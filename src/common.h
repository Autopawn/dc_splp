#ifndef DSA_COMMON_H
#define DSA_COMMON_H

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#if THREADS>0
    #include <pthread.h>
    #include <semaphore.h>
#endif


#define MAX_FACILITIES 1000
#define MAX_CLIENTS 1000
#define MAX_SOL_SIZE 20

typedef long long int lint;
typedef unsigned int uint;
typedef unsigned short ushort;

#define MAX_LINT LLONG_MAX
#define MIN_LINT LLONG_MIN

typedef struct{
    int n_facilities, n_clients;
    // ^ Number of facilities and clients.
    int weights[MAX_CLIENTS];
    // ^ Weight of each client.
    lint distances[MAX_FACILITIES][MAX_CLIENTS];
    // ^ Distance matrix between facilities and clients.
    lint fdistances[MAX_FACILITIES][MAX_FACILITIES];
    // ^ Distance matrix between facilities and facilities, used for solution comparison.
    lint facility_cost[MAX_FACILITIES];
    // ^ Cost of each facility.
    lint transport_cost;
    // ^ Cost of connecting one weight of unit one unit of distance.
    int size_restriction;
    // ^ When it is not -1, the returned solutions must be of that size, also there is no solution filtering in the expansion process.
    lint multiplier;
    // ^ Multiplier of the costs when working with floats.
} problem;

// Auxiliar functions:
void *safe_malloc(size_t size);
uint hash_int(uint x);
void add_to_sorted(short *array, int *len, short val);
void rem_of_sorted(short *array, int *len, short val);

#endif
