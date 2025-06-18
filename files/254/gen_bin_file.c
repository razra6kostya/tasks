#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
    srand(time(NULL));
    int fd, n;
    int rand_len = 1 + (int)(1000.0*rand()/(RAND_MAX+1.0));
    int buf[rand_len];
    int len_buf = sizeof(buf) / sizeof(int);
    if (argc != 2) {
        fprintf(stderr, "Needed one arg.\n"); 
    }
    fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666); 
    
    for (n = 0; n < len_buf; n++) {
        buf[n] = 1 + (int)(1000.0*rand()/(RAND_MAX+1.0));
    }
    write(fd, buf, sizeof(buf)); 
    close(fd);
    return 0;
}
