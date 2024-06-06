# Makefile for process_management.c

# Compiler
CC = gcc

# Compiler Flags (-Wall: Enable all warnings, -Wextra: Enable extra warnings, -std=c99: Standard compiling)
CFLAGS = -Wall -Wextra -std=c99

# Executable Name (name of executable that will be created after compilation of source file)
TARGET = process_management

# Source Files (file to be compiled)
SRC = process_management.c

# Default Target (when make is run without arguments, it will compile process_management)
all: $(TARGET)

# Build the target executable
$(TARGET): $(SRC) # build process_management.exe from process_management.c 
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Clean up build files (erases the executable files)
clean:
	rm -f $(TARGET) *.o

# Run the program (execute using sample_in_process.txt as input)
run:
	./$(TARGET) sample_in_process.txt

# Phony targets
.PHONY: all clean run
