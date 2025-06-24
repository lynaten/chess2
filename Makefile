# Compiler & flags
CC = gcc
CFLAGS = -Wall -Wextra -g -Iheader -Iheader/tree -Isrc

# Output binary name
BIN_FOLDER = bin
TARGET = $(BIN_FOLDER)/chess_sim

# Source folders
SRC_FOLDER = src
CHESS_SRC = $(wildcard $(SRC_FOLDER)/chess/*.c)
TREE_SRC  = $(wildcard $(SRC_FOLDER)/tree/*.c)
GRAPH_SRC  = $(wildcard $(SRC_FOLDER)/graph/*.c)

STDLIB_SRC = $(wildcard $(SRC_FOLDER)/stdlib/*.c)
MAIN_SRC = $(SRC_FOLDER)/main.c

# All sources (includes tree/)
SOURCES = $(CHESS_SRC) $(TREE_SRC) $(STDLIB_SRC) $(MAIN_SRC)

# Object output paths
OBJECTS = $(patsubst $(SRC_FOLDER)/%.c, $(BIN_FOLDER)/%.o, $(SOURCES))

# Ensure bin/ exists
$(BIN_FOLDER):
	@mkdir -p $(BIN_FOLDER)

# Final build target
$(TARGET): $(BIN_FOLDER) $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) -lm

# Compile .c to .o into bin/
$(BIN_FOLDER)/%.o: $(SRC_FOLDER)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(BIN_FOLDER)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Just build
build: $(TARGET)
