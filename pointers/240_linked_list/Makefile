CC = gcc
CFLAGS = -g -Wall

num_work.o: num_work.c num_work.h
	$(CC) $(CFLAGS) -c $< -o $@

task240: task240.c num_work.o
	$(CC) $(CFLAGS) $^ -o $@
