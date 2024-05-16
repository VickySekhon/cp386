# Name of the C compiler to use 
CC = gcc

# Compiler Flags (-wall for warnings, -g for debugging)
CFLAGS = -Wall -g

# Programs (names of the programs to be compiled)
PROGS = directory filecopy

# Default Target (when make is run without arguments, it will compile all programs)
all: $(PROGS)

# Rule for directory program 

# directory compiles directory.c into ./directory 
directory: directory.o
	$(CC) $(CFLAGS) -o directory directory.o

# directory.o compiles directory.c into directory.o
directory.o: directory.c 
	$(CC) $(CFLAGS) -c directory.c

# Rule for filecopy program

# filecopy compiles filecopy.c into ./filecopy
filecopy: filecopy.o
	$(CC) $(CFLAGS) -o filecopy filecopy.o 

# filecopy.o compiles filecopy.c into filecopy.o
filecopy.o: filecopy.c
	$(CC) $(CFLAGS) -c filecopy.c

# Clean rule to remove compiled files
clean:
	rm -f *.o $(PROGS)

# Run directory program
run-directory: directory
	./directory

# Run filecopy program
run-filecopy: filecopy
	./filecopy

# Phony targets
.PHONY: all clean run-directory run-filecopy
