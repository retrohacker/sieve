build: main.c mem.c
	g++ -Wall main.c mem.c
run: build
	./a.out
