CFLAGS = -g -Wall -D DEBAG_PRINT
PROGRAM_NAME = task421
MOD1 = stack_management

$(MOD1).o: $(MOD1).c $(MOD1).h
	gcc $(CFLAGS) -c $(MOD1).c -o $(MOD1).o

run: $(PROGRAM_NAME).c $(MOD1).o
	gcc $(CFLAGS) $(PROGRAM_NAME).c -lm $(MOD1).o -o $(PROGRAM_NAME)

clean:
	rm -f *.o *.swp $(PROGRAM_NAME)
