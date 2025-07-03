CC = gcc
CFLAGS = -g -Wall 
#-fsanitize=address

struct_data.o: struct_data.c struct_data.h
	$(CC) $(CFLAGS) -c $< -o $@

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c $< -o $@

run_cmd.o: run_cmd.c run_cmd.h
	$(CC) $(CFLAGS) -c $< -o $@

prog: main.c struct_data.o parser.o run_cmd.o
	$(CC) $(CFLAGS) $^ -o $@
