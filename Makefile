
all: dsa dsa_ls dsa_hausdorff

bin:
	mkdir -p bin
tests:
	mkdir -p tests
results:
	mkdir -p results

dsa: bin
	cd src; gcc -std=c99 -O -Wall -lm *.c \
		-o ../bin/dsa
dsa_ls: bin
	cd src; gcc -std=c99 -O -Wall -lm *.c \
		-D LOCAL_SEARCH -o ../bin/dsa_ls
dsa_hausdorff: bin
	cd src; gcc -std=c99 -O -Wall -lm *.c \
		-D HAUSDORFF -o ../bin/dsa_hausdorff
dsa_test: results test_problem
	cd src; gcc -std=c99 -O -g -Wall -lm *.c \
		-D LOCAL_SEARCH -D DEBUG -o ../bin/dsa
	valgrind --tool=memcheck --leak-check=yes ./bin/dsa 1000 100 10 \
		tests/simple2_dsa results/simple2_dsa.txt

test_problem: tests
	rm tests/* || true
    # Create test problem:
	python tools/problem_gen.py 20 100 1000 5000 tests/simple
	# Create version with less facility cost:
	sed -e 's/+5000 X/ +2500 X/g' tests/simple_lp > tests/simple2_lp
	sed -e '1c\2500' tests/simple_dsa > tests/simple2_dsa
	# Solve problems with lp_solve
	lp_solve tests/simple_lp > tests/simple_lp_sol
	lp_solve tests/simple2_lp > tests/simple2_lp_sol
	# Plot problem:
	python tools/svg_gen.py tests/simple_pos tests/simple_pos.svg
	convert tests/simple_pos.svg tests/simple_pos.png
	# Plot solution to problem 1:
	python tools/svg_gen.py tests/simple_pos \
		-p tests/simple_lp_sol tests/simple_lp_sol.svg
	convert tests/simple_lp_sol.svg tests/simple_lp_sol.png
	# Plot solution to problem 2:
	python tools/svg_gen.py tests/simple_pos \
		-p tests/simple2_lp_sol tests/simple2_lp_sol.svg
	convert tests/simple2_lp_sol.svg tests/simple2_lp_sol.png
