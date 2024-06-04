# Makefile for process_management.c

# Compiler
CC = gcc

# Compiler Flags (-Wall: Enable all warnings, -Wextra: Enable extra warnings, -std=c99: Standard compiling)
CFLAGS = -Wall -Wextra -std=c99

# Executable Name (name of executable that will be created after compilation of source file)
TARGET = process_management

# Source Files
SRC = process_management.c

# Default Target
all: $(TARGET)

# Build the target executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Clean up build files
clean:
	rm -f $(TARGET) *.o

# Run the program
run:
	./$(TARGET) sample_in_process.txt

# Phony targets
.PHONY: all clean run
