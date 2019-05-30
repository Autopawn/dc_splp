import os

TARGETS = ["common.c","dsa.c","expand.c","load.c","solution.c","reduce_vr.c","reduce_sci.c","reduce.c"]

PARAMETERS = {
    "" : {
        "threads":16,
        "sol_size":100,
        "max_n":2000,
        "max_m":2000,
        "heap_partition":4000,
    },
    "L" : {
        "threads":16,
        "sol_size":550,
        "max_n":3000,
        "max_m":3000,
        "heap_partition":4000,
    },
}

VARIANTS = {
    "dismsemin" : ["REDUCTION_HYBRID", "DISSIM_MSE", "FDISMODE_MINDIST"],
    "dismsesum" : ["REDUCTION_HYBRID", "DISSIM_MSE", "FDISMODE_SUMOFDELTAS"],
    "dishaumin" : ["REDUCTION_HYBRID", "DISSIM_HAUSDORFF", "FDISMODE_MINDIST"],
    "dishausum" : ["REDUCTION_HYBRID", "DISSIM_HAUSDORFF", "FDISMODE_SUMOFDELTAS"],
    "discli"    : ["REDUCTION_HYBRID", "DISSIM_CLIENTDELTA"],
    "scimsemin" : ["REDUCTION_SCI", "DISSIM_MSE", "FDISMODE_MINDIST"],
    "scimsesum" : ["REDUCTION_SCI", "DISSIM_MSE", "FDISMODE_SUMOFDELTAS"],
    "scihaumin" : ["REDUCTION_SCI", "DISSIM_HAUSDORFF", "FDISMODE_MINDIST"],
    "scihausum" : ["REDUCTION_SCI", "DISSIM_HAUSDORFF", "FDISMODE_SUMOFDELTAS"],
    "scicli"    : ["REDUCTION_SCI", "DISSIM_CLIENTDELTA"],
    "bes"       : ["REDUCTION_BESTS"],
    "ran"       : ["REDUCTION_RANDOM"],
}

os.system("mkdir -p bin")

for pa_name,pa in PARAMETERS.items():

    targs = ' '.join(TARGETS)

    for name,macros in VARIANTS.items():
        macros = " ".join(["-D %s"%x for x in macros])

        line = "cd src; \
            gcc -std=c99 -g -D THREADS=%d -Wall %s main.c \
            -D MAX_SOL_SIZE=%d -D MAX_FACILITIES=%d -D MAX_CLIENTS=%d \
            -D HEAP_PARTITION=%d \
            -D LOCAL_SEARCH \
            %s \
            -o ../bin/dc_%s%s -lm -lpthread"%(
            pa['threads'],targs,
            pa['sol_size'],pa['max_n'],pa['max_m'],
            pa['heap_partition'],
            macros,name,pa_name)
        print(line)
        os.system(line)
