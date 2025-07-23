CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
# -Wall: Enable all common warnings
# -Wextra: Enable extra warnings
# -Werror: Treat warnings as errors
# -std=c11: Use C11 standard
# -g: Include debugging information (for valgrind and GDB)

# Executable name
TARGET = shell

# List of all source .c files
SRCS = main.c lexer.c parser.c mem_utils.c utils.c

# List of all object .o files (automatically generated from SRCS)
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

# Default rule: builds everything and creates the executable
all: $(TARGET)

# Rule for building the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Rule for compiling each .c file into a .o file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for cleaning the project (removes object files and executable)
clean:
	rm -f $(OBJS) $(TARGET)
