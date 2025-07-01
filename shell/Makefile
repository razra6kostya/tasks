CC = gcc
CFLAGS = -g -Wall 
#-fsanitize=address

wlist_manager.o: wlist_manager.c wlist_manager.h
	$(CC) $(CFLAGS) -c $< -o $@

prog: main.c wlist_manager.o
	$(CC) $(CFLAGS) $^ -o $@
