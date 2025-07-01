CC = gcc
CFLAGS = -g -Wall 
#-fsanitize=address

wlist_manager.o: wlist_manager.c wlist_manager.h
	$(CC) $(CFLAGS) -c $< -o $@

cmd_manager.o: cmd_manager.c cmd_manager.h
	$(CC) $(CFLAGS) -c $< -o $@

prog: main.c word_list.h wlist_manager.o cmd_manager.o
	$(CC) $(CFLAGS) $^ -o $@
