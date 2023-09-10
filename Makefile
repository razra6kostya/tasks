CFLAGS = -g -Wall

mod.o: mod.c mod.h
	gcc $(CFLAGS) -c mod.c -o mod.o

prog: task240.c mod.o
	gcc $(CFLAGS) task240.c mod.o -o task240

clean:
	rm -f *.o task240
