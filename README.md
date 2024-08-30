# Overview
## A01 - File System Manager Simulator:
```directory.c``` - contains functions that are used to create, delete, and list directories.\
```filecopy.c``` - uses built-in POSIX libraries to copy contents of one file to another.

## A02 - Process Synchronization:
```collatz_sequence.c``` - contains a program that uses a parent process to create and execute a child process which prints the collatz sequence for a number read from the start_number.txt file.
```process_management.c``` - contains a program that uses multiple parent, child processes and a grandchild process which:

 1) write POSIX commands to a shared memory object
 2) read from a shared memory object to tokenize commands one-by-one
 3) use a ```pipe``` to communicate between grandchild-child-parent processes
 4) create dynamic processes to execute each POSIX command independently
 5) writes the associated command and it's output to a file marked 'output.txt'

## A03 - Non-blocking Thread Creation:
```sudoku.c``` - uses 11 individual threads to validate a 9x9 Sudoku puzzle based on it's rows, columns, and 3x3 subgrids.
```fcfs.c``` - This program reads the Thread ID, Arrival Time, and Burst Time from a file and

"simulates" scheduling these algorithms based on the First Come First Serve (FCFS)
CPU scheduling algorithm. The threads in the file "sample_in_schedule.txt" are
non-preemptive to allow for the simulation of the FCFS algorithm.

The program calculates the completion time, turn-around time, and waiting time
of the threads using the following formulas:

• Completion time = (Burst Time n) + (Burst Time n-1) + ... + (Burst Time 1)
• Turn-around time = Completion time - Arrival time
• Waiting time = Turn-around time - Burst Time

The average waiting time and turn-around times are then calculated.

## A04 - Manual Memory Allocation in C:
```allocation.c``` -  This file includes the First Fit Contiguous memory allocation algorithm to simulate OS resource allocation. 

# Credits
Code for Operating Systems, written in C

By Vicky Sekhon and Yafet Tegbaru
