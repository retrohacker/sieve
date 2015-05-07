build: main.c mem.c
	g++ -Wall -ggdb -fopenmp main.c mem.c
run: build
	./a.out
test: build
	valgrind --leak-check=full ./a.out
