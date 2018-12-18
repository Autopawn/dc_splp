targets = common.c dsa.c expand.c load.c solution.c reduce.c

THREADS = 4

all: dsa dsa_ls dsa_hausdorff randomhc

bin:
	mkdir -p bin
tests:
	mkdir -p tests
dsa: bin
	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
		-o ../bin/dsa -lm -lpthread
dsa_ls: bin
	cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) main.c \
		-D LOCAL_SEARCH -o ../bin/dsa_ls -lm -lpthread
randomhc: bin
		cd src; gcc -std=c99 -g -D THREADS=$(THREADS) -Wall $(targets) mainhc.c \
		-o ../bin/randomhc -lm -lpthread
dsa_hausdorff: bin
	cd src; gcc -std=c99 -g -O -Wall $(targets) main.c \
		-D HAUSDORFF -o ../bin/dsa_hausdorff -lm
dsa_test: dsa_ls tests/simple_pos
	valgrind ./bin/dsa_ls 100 200 10 tests/prob_p5_dsa_pm tests/res_dsa.txt tests/res_dsa_ls.txt
dsa_time: dsa_ls tests/simple_pos
	time ./bin/dsa_ls 100 200 10 tests/prob_p5_dsa_pm tests/res_dsa.txt tests/res_dsa_ls.txt
dsa_profile: dsa_ls tests/simple_pos
	valgrind --tool=callgrind --callgrind-out-file=tests/callgrind.out.timetest \
		./bin/dsa_ls 100 200 10 tests/prob_p5_dsa_pm tests/res_dsa.txt tests/res_dsa_ls.txt
	callgrind_annotate --tree=caller tests/callgrind.out.timetest > tests/parallel_$(THREADS).txt
tests/simple_pos: tests
	# Create test template (n=50)
	python tools/template_gen.py 50 50 10000 tests/simple
	# Create p-median problem (p=15):
	sed -e "s/<<PP>>/15/g" tests/simple_dsa_pm > tests/prob_p5_dsa_pm
	# Plot problem:
	python tools/svg_gen.py tests/simple_pos tests/prob.svg
dsa_ls_many: bin tests
	for i in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16; do \
		cd src; gcc -std=c99 -g -D THREADS=$$i -Wall $(targets) main.c \
		-D LOCAL_SEARCH -o ../bin/dsa_ls_$$i -lm -lpthread; \
		cd ..; ./bin/dsa_ls_$$i 150 300 10 tests/prob_p5_dsa_pm tests/res_dsa_$$i.txt tests/res_dsa_ls_$$i.txt; \
	done

# fname = problems/prob_n0050_i0001_p08_dsa_splp
# solname = result/prob_n0050_i0001_p08_dsa_splp
# timetest:
# 	rm -rf result 2>/dev/null
# 	mkdir -p result
# 	valgrind --tool=callgrind --callgrind-out-file=callgrind.out.timetest ./bin/dsa_ls 100 200 1 $(fname) $(solname) $(solname)hc
# 	callgrind_annotate --tree=caller callgrind.out.timetest > profiles/no_parallel.txt
