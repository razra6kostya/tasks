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

    const char* filename = argv[1];
    long start_position = atol(argv[2]);
    long length = atol(argv[3]);
    unsigned char byte_value = atol(argv[4]);

    const char *usage_message =
        "Usage: %s <filename> <start_position> <length> <byte_value>\n";
    const char *invalid_args_message =
        "Error: Invalid arguments. start_position and length must be "
        "non-negative. byte_value must be between 0 and 255.\n";

    if (argc != 5) {
        fprintf(stderr, usage_message, argv[0]);
        return 1;
    }
    if (start_position < 0 || length < 0 || 
        byte_value < 0 || byte_value > 255) {
        fprintf(stderr, invalid_args_message); 
        return 2;
    }
    fd = open(filename, O_RDWR | O_CREAT, 0666); 
    if (fd == -1) {
        perror(filename);
        return 3;
    }
    if (lseek(fd, start_position, SEEK_SET) == -1) {
        perror("Error seeking to position");
        close(fd);
        return 4;
    }
        if (length == 0) {
        close(fd);
        return 0;
    }

    if (length >= BLOCK_SIZE) {
        buffer = (unsigned char *)malloc(BLOCK_SIZE);
        if (buffer == NULL) {
            perror("Error allocating buffer"); 
            close(fd);
            return 5;
        }
        memset(buffer, byte_value, BLOCK_SIZE);
        bytes_written = 0;
        while (BLOCK_SIZE < length) {
            bytes_written = write(fd, buffer, BLOCK_SIZE);
            length -= bytes_written;
            // onse again
        }
        free(buffer);
    }
    if (length > 0) {
        buffer = (unsigned char *)malloc(length);
        memset(buffer, byte_value, length);
        bytes_written = write(fd, buffer, length);
        if (bytes_written < 0) {
            perror("Error writing to file");
            free(buffer);
            close(fd);
            return 6;
        }
    }
    free(buffer);
    close(fd);
    return 0;
}
