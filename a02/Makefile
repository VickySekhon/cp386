# Makefile for collatz_sequence.c

# Compiler
CC = gcc

# Compiler Flags (-Wall: Enable all warnings, -Wextra: Enable extra warnings, -std=c99: Standard compiling)
CFLAGS = -Wall -Wextra -std=c99

# Executable Name (name of executable that will be created after compilation of source file)
TARGET = collatz_sequence

# Source Files (file to be compiled)
SRC = collatz_sequence.c

# Default Target (when make is run without arguments, it will compile collatz_sequence)
all: $(TARGET)

# Build the target executable
$(TARGET): $(SRC) # build collatz_sequence.exe from collatz_sequence.c 
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Clean up build files (erases the executable and object files)
clean:
	rm -f $(TARGET) *.o

# Run the program (execute using start_numbers.txt as input)
run:
	./$(TARGET) start_numbers.txt

# Phony targets
.PHONY: all clean run
