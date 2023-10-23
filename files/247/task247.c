#include <stdio.h>

#define MODE "r"


int main(int argc, char** argv)
{
    FILE *f;
    int c;
    if (argc != 2) {
        fprintf(stderr,"Needed argument\n");
        return 1; 
    }
    f = fopen(argv[1], MODE); 
    if (!f) {
        perror(argv[1]);
        return 2;
    }
    while ((c = fgetc(f)) != EOF) {
        putchar(c);
    }
    fclose(f);
    return 0;
}
