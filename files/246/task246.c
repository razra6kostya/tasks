#include <stdio.h>

#define MODE "w"

char *text = "Humpty Dumpty sat on a wall,\n"
             "Humpty Dumpty had a great fall;\n"
             "All the king's horses and all the king's men\n"
             "Couldn't put Humpty together again.\n";

int main(int argc, char** argv)
{
    FILE *f;
    if (argc != 2) {
        fprintf(stderr,"Needed argument\n");
        return 1; 
    }
    f = fopen(argv[1], MODE); 
    if (!f) {
        perror(argv[1]);
        return 2;
    }
    while (fputc(*text++, f)) {
        ;
    }
    fclose(f);
    return 0;
}
