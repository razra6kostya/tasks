#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>

void analyze_bin_file(int fd, int *count, int *min, int *max, int *len_str)
{
    int i, read_byte, buf[1024];
    while ((read_byte = read(fd, buf, sizeof(buf)))) {
        for (i = 0; i < read_byte / sizeof(int); i++) {
            (*count)++;
            if (i == 0) {
                *min = *max = buf[i];
            }
            if (buf[i] < *min) {
                *min = buf[i];
            }
            if (buf[i] > *max) {
                *max = buf[i];
            }
        }
    }
    *len_str = snprintf(NULL, 0, "%d:%d:%d", *count, *min, *max) + 1; 
}


int main(int argc, char **argv)
{
    FILE *analyze;
    int fd;
    int i;
    int len_str, count, min, max;
    char *str = NULL;
    
    if (argc < 3) {
        fprintf(stderr, "Too few arguments\n");
        return 1;
    }
     
    analyze = fopen(argv[argc-1], "w");
    if (!analyze) {
        perror(argv[argc-1]);
        return 2;
    }
    for (i = 1; i < argc - 1; i++) {
        fd = open(argv[i], O_RDONLY);
        if (fd < 0) {
            perror(argv[i]);
            return 3;
        } 
        len_str = count = min = max = 0;
        analyze_bin_file(fd, &count, &min, &max, &len_str);
        str = malloc(sizeof(char) * len_str);
        sprintf(str, "%d:%d:%d", count, min, max);
        fprintf(analyze, "%s:%s\n", argv[i], str);
        free(str);
        close(fd);
    }
    fclose(analyze);
    return 0;
}
