targets = common.c dsa.c expand.c load.c solution.c reduce.c

THREADS = 16

all: dsa_ls dsa_bests dsa_hausdorff randomhc many_bin

bin:
	mkdir -p bin
tests:
	mkdir -p tests
# dsa: bin
# 	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
# 		-o ../bin/dsa -lm -lpthread
dsa_ls: bin
	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
		-D LOCAL_SEARCH -o ../bin/dsa_ls -lm -lpthread
dsa_bests: bin
	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
		-D LOCAL_SEARCH -D ONLY_BESTS -o ../bin/dsa_best -lm -lpthread
randomhc: bin
		cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) mainhc.c \
		-o ../bin/randomhc -lm -lpthread
dsa_hausdorff: bin
	cd src; gcc -std=c99 -g -O -Wall $(targets) main.c \
		-D HAUSDORFF -D LOCAL_SEARCH -o ../bin/dsa_hausdorff_ls -lm
many_bin: bin tests
	for i in 0 1 2 3 4 5 6 7 8 9 10; do \
		cd src; gcc -std=c99 -g -D THREADS=$$i -Wall $(targets) main.c \
		-D LOCAL_SEARCH -o ../bin/dsa_ls_$$i -lm -lpthread; \
		cd ..; \
	done

# fname = problems/prob_n0050_i0001_p08_dsa_splp
# solname = result/prob_n0050_i0001_p08_dsa_splp
# timetest:
# 	rm -rf result 2>/dev/null
# 	mkdir -p result
# 	valgrind --tool=callgrind --callgrind-out-file=callgrind.out.timetest ./bin/dsa_ls 100 200 1 $(fname) $(solname) $(solname)hc
# 	callgrind_annotate --tree=caller callgrind.out.timetest > profiles/no_parallel.txt

# Good testing one: "splp/Euclid/1311EuclS.txt"
