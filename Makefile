all: memgrind.c my_malloc.o
	gcc -o memgrind memgrind.c my_malloc.o

my_malloc.o: my_malloc.c
	gcc -o my_malloc.o -c my_malloc.c

clean:
	rm -rf memgrind
