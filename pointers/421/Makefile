CC = gcc
CFLAGS = -Wall -g
stack_management.o: stack_management.c stack_management.h
	$(CC) $(CFLAGS) -c $< -o $@

task421: stack_management.o task421.c
	$(CC) $(CFLAGS) $^ -lm -o $@
