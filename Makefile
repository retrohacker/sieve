build: main.c mem.c
	g++ -Wall -fopenmp main.c mem.c
run: build
	./a.out
test: build
	valgrind --leak-check=yes ./a.out
