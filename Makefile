targets = common.c dsa.c expand.c load.c solution.c reduce.c

THREADS = 16
SMALL_SOL_SIZE = 100
LARGE_SOL_SIZE = 500
SMALL_N = 2000
SMALL_M = 2000
LARGE_N = 3000
LARGE_M = 3000

all: dc_dismsemin dc_dismsesum dc_dishaumin dc_dishausum dc_discli dc_bes dc_ran \
	dc_dismseminL dc_dismsesumL dc_dishauminL dc_dishausumL dc_discliL dc_besL dc_ranL \
	randomhc dc_disclione dc_disclinwi

bin:
	mkdir -p bin
tests:
	mkdir -p tests
dc_dismsemin: bin
	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
		-D MAX_SOL_SIZE=$(SMALL_SOL_SIZE) -D MAX_FACILITIES=$(SMALL_N) -D MAX_CLIENTS=$(SMALL_M) \
		-D LOCAL_SEARCH \
		-D REDUCTION_DISPERSE -D DISSIM_MSE -D FDISMODE_MINDIST \
		-o ../bin/dc_dismsemin -lm -lpthread
dc_dismseminL: bin
	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
		-D MAX_SOL_SIZE=$(LARGE_SOL_SIZE) -D MAX_FACILITIES=$(LARGE_N) -D MAX_CLIENTS=$(LARGE_M) \
		-D LOCAL_SEARCH \
		-D REDUCTION_DISPERSE -D DISSIM_MSE -D FDISMODE_MINDIST \
		-o ../bin/dc_dismseminL -lm -lpthread
dc_dismsesum: bin
	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
		-D MAX_SOL_SIZE=$(SMALL_SOL_SIZE) -D MAX_FACILITIES=$(SMALL_N) -D MAX_CLIENTS=$(SMALL_M) \
		-D LOCAL_SEARCH \
		-D REDUCTION_DISPERSE -D DISSIM_MSE -D FDISMODE_SUMOFDELTAS \
		-o ../bin/dc_dismsesum -lm -lpthread
dc_dismsesumL: bin
	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
		-D MAX_SOL_SIZE=$(LARGE_SOL_SIZE) -D MAX_FACILITIES=$(LARGE_N) -D MAX_CLIENTS=$(LARGE_M) \
		-D LOCAL_SEARCH \
		-D REDUCTION_DISPERSE -D DISSIM_MSE -D FDISMODE_SUMOFDELTAS \
		-o ../bin/dc_dismsesumL -lm -lpthread
dc_dishaumin: bin
	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
		-D MAX_SOL_SIZE=$(SMALL_SOL_SIZE) -D MAX_FACILITIES=$(SMALL_N) -D MAX_CLIENTS=$(SMALL_M) \
		-D LOCAL_SEARCH \
		-D REDUCTION_DISPERSE -D DISSIM_HAUSDORFF -D FDISMODE_MINDIST \
		-o ../bin/dc_dishaumin -lm -lpthread
dc_dishauminL: bin
	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
		-D MAX_SOL_SIZE=$(LARGE_SOL_SIZE) -D MAX_FACILITIES=$(LARGE_N) -D MAX_CLIENTS=$(LARGE_M) \
		-D LOCAL_SEARCH \
		-D REDUCTION_DISPERSE -D DISSIM_HAUSDORFF -D FDISMODE_MINDIST \
		-o ../bin/dc_dishauminL -lm -lpthread
dc_dishausum: bin
	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
		-D MAX_SOL_SIZE=$(SMALL_SOL_SIZE) -D MAX_FACILITIES=$(SMALL_N) -D MAX_CLIENTS=$(SMALL_M) \
		-D LOCAL_SEARCH \
		-D REDUCTION_DISPERSE -D DISSIM_HAUSDORFF -D FDISMODE_SUMOFDELTAS \
		-o ../bin/dc_dishausum -lm -lpthread
dc_dishausumL: bin
	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
		-D MAX_SOL_SIZE=$(LARGE_SOL_SIZE) -D MAX_FACILITIES=$(LARGE_N) -D MAX_CLIENTS=$(LARGE_M) \
		-D LOCAL_SEARCH \
		-D REDUCTION_DISPERSE -D DISSIM_HAUSDORFF -D FDISMODE_SUMOFDELTAS \
		-o ../bin/dc_dishausumL -lm -lpthread
dc_discli: bin
	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
		-D MAX_SOL_SIZE=$(SMALL_SOL_SIZE) -D MAX_FACILITIES=$(SMALL_N) -D MAX_CLIENTS=$(SMALL_M) \
		-D LOCAL_SEARCH \
		-D REDUCTION_DISPERSE -D DISSIM_CLIENTDELTA \
		-o ../bin/dc_discli -lm -lpthread
dc_discliL: bin
	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
		-D MAX_SOL_SIZE=$(LARGE_SOL_SIZE) -D MAX_FACILITIES=$(LARGE_N) -D MAX_CLIENTS=$(LARGE_M) \
		-D LOCAL_SEARCH \
		-D REDUCTION_DISPERSE -D DISSIM_CLIENTDELTA \
		-o ../bin/dc_discliL -lm -lpthread
dc_bes: bin
	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
		-D MAX_SOL_SIZE=$(SMALL_SOL_SIZE) -D MAX_FACILITIES=$(SMALL_N) -D MAX_CLIENTS=$(SMALL_M) \
		-D LOCAL_SEARCH \
		-D REDUCTION_BESTS \
		-o ../bin/dc_bes -lm -lpthread
dc_besL: bin
	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
		-D MAX_SOL_SIZE=$(LARGE_SOL_SIZE) -D MAX_FACILITIES=$(LARGE_N) -D MAX_CLIENTS=$(LARGE_M) \
		-D LOCAL_SEARCH \
		-D REDUCTION_BESTS \
		-o ../bin/dc_besL -lm -lpthread
dc_ran: bin
	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
		-D MAX_SOL_SIZE=$(SMALL_SOL_SIZE) -D MAX_FACILITIES=$(SMALL_N) -D MAX_CLIENTS=$(SMALL_M) \
		-D LOCAL_SEARCH \
		-D REDUCTION_RANDOM \
		-o ../bin/dc_ran -lm -lpthread
dc_ranL: bin
	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
		-D MAX_SOL_SIZE=$(LARGE_SOL_SIZE) -D MAX_FACILITIES=$(LARGE_N) -D MAX_CLIENTS=$(LARGE_M) \
		-D LOCAL_SEARCH \
		-D REDUCTION_RANDOM \
		-o ../bin/dc_ranL -lm -lpthread
randomhc: bin
		cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) mainhc.c \
		-D MAX_SOL_SIZE=$(SMALL_SOL_SIZE) -D MAX_FACILITIES=$(SMALL_N) -D MAX_CLIENTS=$(SMALL_M) \
		-D RANDOMHC \
		-D REDUCTION_BESTS \
		-o ../bin/randomhc -lm -lpthread
dc_disclione: bin
	cd src; gcc -std=c99 -g -D THREADS=0 -Wall $(targets) main.c \
		-D MAX_SOL_SIZE=$(SMALL_SOL_SIZE) -D MAX_FACILITIES=$(SMALL_N) -D MAX_CLIENTS=$(SMALL_M) \
		-D LOCAL_SEARCH \
		-D REDUCTION_DISPERSE -D DISSIM_CLIENTDELTA \
		-o ../bin/dc_disclione -lm -lpthread
dc_disclinwi: bin
	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
		-D MAX_SOL_SIZE=$(SMALL_SOL_SIZE) -D MAX_FACILITIES=$(SMALL_N) -D MAX_CLIENTS=$(SMALL_M) \
		-D LOCAL_SEARCH -D LOCALSEARCH_DONT_USE_WHITAKER \
		-D REDUCTION_DISPERSE -D DISSIM_CLIENTDELTA \
		-o ../bin/dc_disclinwi -lm -lpthread

# dsa: bin
# cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
# -o ../bin/dsa -lm -lpthread

# dc_singlesum: bin
# 	cd src; gcc -std=c99 -g -D THREADS=0 -Wall $(targets) main.c \
# 		-D LOCAL_SEARCH \
# 		-D FDIS_SUM_MODE \
# 		-o ../bin/dc_singlesum -lm -lpthread

# dc_norm_s: bin
# 	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
# 		-D LOCAL_SEARCH -D FDIST_SUM_MODE \
# 		-o ../bin/dc_norm_s -lm -lpthread
# dc_best_m: bin
# 	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
# 		-D LOCAL_SEARCH -D REDUCTION_BESTS \
# 		-o ../bin/dc_best_m -lm -lpthread
# dc_best_s: bin
# 	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
# 		-D LOCAL_SEARCH -D FDIST_SUM_MODE -D  \
# 		-o ../bin/dc_best_s -lm -lpthread
# dc_haus_m: bin
# 	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
# 		-D LOCAL_SEARCH -D REDUCTION_HAUSDORFF \
# 		-o ../bin/dc_haus_m -lm -lpthread
# dc_haus_s: bin
# 	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
# 		-D LOCAL_SEARCH -D FDIST_SUM_MODE -D REDUCTION_HAUSDORFF \
# 		-o ../bin/dc_haus_s -lm -lpthread

# dc_nonw_m: bin
# 	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
# 	-D LOCAL_SEARCH -D LOCALSEARCH_DONT_USE_WHITAKER \
# 	-o ../bin/dc_nonw_m -lm -lpthread

# many_bin: bin tests
# 	for i in 0 1 2 3 4 5 6 7 8 9 10; do \
# 		cd src; gcc -std=c99 -g -D THREADS=$$i -Wall $(targets) main.c \
# 		-D LOCAL_SEARCH -o ../bin/dsa_ls_$$i -lm -lpthread; \
# 		cd ..; \
# 	done

# fname = problems/prob_n0050_i0001_p08_dsa_splp
# solname = result/prob_n0050_i0001_p08_dsa_splp
# timetest:
# 	rm -rf result 2>/dev/null
# 	mkdir -p result
# 	valgrind --tool=callgrind --callgrind-out-file=callgrind.out.timetest ./bin/dsa_ls 100 200 1 $(fname) $(solname) $(solname)hc
# 	callgrind_annotate --tree=caller callgrind.out.timetest > profiles/no_parallel.txt

# Good testing one: "splp/Euclid/1311EuclS.txt"
