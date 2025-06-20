#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 4096

int main(int argc, char **argv)
{
    int fd;
    unsigned char *buffer;
    int bytes_written;
    char *endptr;
    long long start_position, length;
    unsigned char byte_value;

    const char* filename = argv[1];
    const char *usage_message =
        "Usage: %s <filename> <start_position> <length> <byte_value>\n";

    start_position = strtoll(argv[2], &endptr, 10);
    if (*endptr != '\0' || start_position < 0) {
        fprintf(stderr, "Invalid start_position: %s\n", argv[2]);
        return 1;
    }
    length = strtol(argv[3], &endptr, 10); 
    if (*endptr != '\0' || length < 0) {
        fprintf(stderr, "Invalid length: %s\n", argv[3]);
        return 2;
    }
    byte_value = strtol(argv[4], &endptr, 10); 
    if (*endptr != '\0' || byte_value < 0 || byte_value > 255) {
        fprintf(stderr, "Invalid byte_value: %s (must be 0-255)\n", argv[4]);
        return 3;
    }
    if (argc != 5) {
        fprintf(stderr, usage_message, argv[0]);
        return 4;
    }
    fd = open(filename, O_RDWR | O_CREAT, 0666); 
    if (fd == -1) {
        perror(filename);
        return 5;
    }
    if (lseek(fd, start_position, SEEK_SET) == -1) {
        perror("Error seeking to position");
        close(fd);
        return 6;
    }
        if (length == 0) {
        close(fd);
        return 0;
    }

    buffer = (unsigned char *)malloc(BLOCK_SIZE);
    if (buffer == NULL) {
        perror("Error allocating buffer"); 
        close(fd);
        return 7;
    }

    memset(buffer, byte_value, BLOCK_SIZE);
    if (length >= BLOCK_SIZE) {
        bytes_written = 0;
        while (BLOCK_SIZE <= length) {
            bytes_written = write(fd, buffer, BLOCK_SIZE);
            length -= bytes_written;
        }
    }
    if (length > 0) {
        bytes_written = write(fd, buffer, length);

        if (bytes_written < 0) {
            perror("Error writing to file");
            free(buffer);
            close(fd);
            return 8;
        }
    }
    free(buffer);
    close(fd);
    return 0;
}
