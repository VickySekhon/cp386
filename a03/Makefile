# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c99 -pthread

# Target executables
TARGETS = fcfs sudoku

# Source files
SRC_FCFS = fcfs.c
SRC_SUDOKU = sudoku.c

# Default target
all: $(TARGETS)

# Build fcfs target
fcfs: $(SRC_FCFS)
	$(CC) $(CFLAGS) -o fcfs $(SRC_FCFS)

# Build sudoku target
sudoku: $(SRC_SUDOKU)
	$(CC) $(CFLAGS) -o sudoku $(SRC_SUDOKU)

# Clean target
clean:
	rm -f $(TARGETS)

# Run fcfs target
run_fcfs: fcfs
	./fcfs

# Run sudoku target
run_sudoku: sudoku
	./sudoku

.PHONY: all clean run_fcfs run_sudoku
