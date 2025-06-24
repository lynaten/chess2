# Compiler
CC = gcc

# Compiler flags
# -Wall: Enable all warnings
# -g: Generate debugging information
CFLAGS = -Wall -g -std=c99

# The target executable name
TARGET = graph_parser

# The source file
SRC = graph_parser.c

# Default rule, executed when you run `make`
all: $(TARGET)

# Rule to link the object file into the final executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Rule to clean up the build directory
clean:
	rm -f $(TARGET)

# Phony targets are not actual files
.PHONY: all clean
