CFLASG = -g -Wall
PROGRAM_NAME = task243
MOD1 = word_pointer

$(MOD1).o: $(MOD1).c $(MOD1).h
	gcc $(CFLASG) -c $(MOD1).c -o $(MOD1).o

run: $(PROGRAM_NAME).c $(MOD1).o
	gcc $(CFLASG) $(PROGRAM_NAME).c $(MOD1).o -o $(PROGRAM_NAME)

clean:
	rm -f *.o $(PROGRAM_NAME)
