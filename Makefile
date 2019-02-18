targets = common.c dsa.c expand.c load.c solution.c reduce.c

THREADS = 16

all: dc_normmin dc_normsum dc_hausmin dc_haussum dc_best dc_rand dc_singlesum randomhc

bin:
	mkdir -p bin
tests:
	mkdir -p tests
# dsa: bin
# 	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
# 		-o ../bin/dsa -lm -lpthread
dc_normmin: bin
	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
		-D LOCAL_SEARCH \
		-o ../bin/dc_normmin -lm -lpthread
dc_normsum: bin
	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
		-D LOCAL_SEARCH -D FDIS_SUM_MODE \
		-o ../bin/dc_normsum -lm -lpthread
dc_hausmin: bin
	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
		-D LOCAL_SEARCH -D REDUCTION_HAUSDORFF \
		-o ../bin/dc_hausmin -lm -lpthread
dc_haussum: bin
	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
		-D LOCAL_SEARCH -D REDUCTION_HAUSDORFF -D FDIS_SUM_MODE \
		-o ../bin/dc_haussum -lm -lpthread
dc_best:
	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
		-D LOCAL_SEARCH -D REDUCTION_BESTS \
		-o ../bin/dc_best -lm -lpthread
dc_rand:
	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
		-D LOCAL_SEARCH -D REDUCTION_RANDOM \
		-o ../bin/dc_rand -lm -lpthread
dc_singlesum: bin
	cd src; gcc -std=c99 -g -D THREADS=0 -Wall $(targets) main.c \
		-D LOCAL_SEARCH -D FDIS_SUM_MODE \
		-o ../bin/dc_singlesum -lm -lpthread


dc_norm_m: bin

dc_norm_s: bin
	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
		-D LOCAL_SEARCH -D FDIST_SUM_MODE \
		-o ../bin/dc_norm_s -lm -lpthread
dc_best_m: bin
	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
		-D LOCAL_SEARCH -D REDUCTION_BESTS \
		-o ../bin/dc_best_m -lm -lpthread
dc_best_s: bin
	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
		-D LOCAL_SEARCH -D FDIST_SUM_MODE -D  \
		-o ../bin/dc_best_s -lm -lpthread
dc_haus_m: bin
	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
		-D LOCAL_SEARCH -D REDUCTION_HAUSDORFF \
		-o ../bin/dc_haus_m -lm -lpthread
dc_haus_s: bin
	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
		-D LOCAL_SEARCH -D FDIST_SUM_MODE -D REDUCTION_HAUSDORFF \
		-o ../bin/dc_haus_s -lm -lpthread
# dc_nonw_m: bin
# 	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
# 	-D LOCAL_SEARCH -D DONT_USE_WHITAKER \
# 	-o ../bin/dc_nonw_m -lm -lpthread

randomhc: bin
		cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) mainhc.c \
		-o ../bin/randomhc -lm -lpthread

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
