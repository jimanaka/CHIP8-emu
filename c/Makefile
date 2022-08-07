CC = gcc
FLAGS = -Wall

emulate: main.o
	$(CC) main.o -o emulate

main.o: main.c
	$(CC) $(FLAGS) -c -g main.c -o main.o

clean:
	rm -rf *.o emulate

