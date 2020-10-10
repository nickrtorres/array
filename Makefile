.POSIX:

CFLAGS = -O3

main: main.o array.o

check: main
	./main test

bench: main
	./main bench

clean:
	rm -f main main.o array.o
	rm -rf main.dSYM

array.o: array.c
main.o: main.c

