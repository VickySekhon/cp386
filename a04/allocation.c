/*
 -------------------------------------
 File: allocation.c
 Project: sekh4498_a02, tegb0140_a02

 This project uses the First Fit Conti-
 -guous memory allocation algorithm to
 simulate OS resource allocation. 
 -------------------------------------
 Authors:  Vicky Sekhon, Yafet Tegbaru
 IDs:      169024498, 210480140
 Emails:   sekh4498@mylaurier.ca, tegb0140@mylaurier.ca
 Version   2024-07-15
 -------------------------------------
 */

// TODO is used to indicate different sections of the code as it is part of a VSCODE extension that makes it easy to differenciate sections due to colour coding.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: constants

#define MAX 1048576               // Default memory size
#define COMMAND_LINE_PARAMETERS 2 // one command-line that specifies the size of allocated memory in bytes
#define BUFFER 100                // general buffer

// TODO: types and global variables

typedef enum
{
    False,
    True
} Boolean;

typedef struct
{
    int start;            // location where memory memory starts
    int size;             // size of memory
    char processName[20]; // unique identifier for a memory memory
    Boolean allocated;    // tracks whether or not memory allocation was successful
} MemoryBlock;

// make memory block global so that it is accessible by all functions
MemoryBlock memory[MAX];
int memorySize = MAX;

// TODO: function prototypes

// user requests
int exitRequest(char *);
int allocationRequest(char *);
int releaseRequest(char *);
int compactionRequest(char *);
int statusRequest(char *);
int validAlgorithmRequest(char *);

// initialization
void initializeMemory();

// extra functions
int matchingProcesses(char *, char *);

// request implementations
void compactMemory();
void printMemoryStatus();
void releaseMemory(char *);
Boolean requestMemory(char *, int);

// TODO: driver
int main(int argc, char *argv[])
{
    if (argc != COMMAND_LINE_PARAMETERS)
    {
        printf("Insufficient command line parameters passed. Cannot proceed.\n");
        exit(1); // exit failure
    }

    // get memory size and convert it to int
    memorySize = atoi(argv[1]);

    // setup main memory source
    initializeMemory();

    char userInput[BUFFER];

    while (1)
    {
        printf("allocator>");
        fgets(userInput, sizeof(userInput), stdin); // get user input

        if (exitRequest(userInput) == 0)
        {
            break;
        }
        else if (allocationRequest(userInput) == 0)
        {
            char processName[BUFFER];
            int size;
            char algorithm[2]; // contiguous memory allocation algorithm (First Fit, Best Fit, Worst Fit)

            // get process ID, size of memory memory, and algorithm of memory block to allocate
            sscanf(userInput, "RQ %s %d %s", processName, &size, algorithm);

            // selected "First Fit (F)" algorithm
            if (validAlgorithmRequest(algorithm) == 0)
            {
                requestMemory(processName, size);
            }
            else
            {
                printf("Unknown allocation algorithm\n");
            }
        }
        else if (releaseRequest(userInput) == 0)
        {
            char processName[20];

            sscanf(userInput, "RL %s", processName); // get process id to release

            releaseMemory(processName);
        }
        else if (compactionRequest(userInput) == 0)
        {
            compactMemory();
        }
        else if (statusRequest(userInput) == 0)
        {
            printMemoryStatus();
        }
        else
        {
            printf("Unknown command entered. Please try again.\n");
        }
    }

    return 0;
}

// function implementations

// Initialize memory with one large free block
void initializeMemory()
{
    memory[0].start = 0;
    memory[0].size = memorySize;
    memory[0].allocated = False;
    strcpy(memory[0].processName, "");
}

int exitRequest(char *userRequest)
{
    return strcmp(userRequest, "Exit\n");
}

int allocationRequest(char *userRequest)
{
    return strncmp(userRequest, "RQ", 2);
}

int releaseRequest(char *userRequest)
{
    return strncmp(userRequest, "RL", 2);
}

int compactionRequest(char *userRequest)
{
    return strcmp(userRequest, "C\n");
}

int statusRequest(char *userRequest)
{
    return strcmp(userRequest, "Status\n");
}

int validAlgorithmRequest(char *algorithm)
{
    return strcmp(algorithm, "F"); // this program only supports the first fit contiguous allocation method
}

int matchingProcesses(char *currentProcess, char *processToMatch)
{
    return strcmp(currentProcess, processToMatch);
}

void releaseMemory(char *processName)
{
    // go through all memory
    for (int i = 0; i < memorySize; i++)
    {
        // found matching process ID to the one that must be deleted
        if (memory[i].allocated && matchingProcesses(memory[i].processName, processName) == 0)
        {
            // deallocate memory
            memory[i].allocated = False;

            strcpy(memory[i].processName, "");

            printf("Releasing memory for process %s\n", processName);

            return;
        }
    }
    // could not successfully find process
    printf("Process %s not found\n", processName);
    return;
}

Boolean requestMemory(char *processName, int size)
{
    // search through all memory blocks
    for (int i = 0; i < memorySize; i++)
    {
        // free memory block that can accommodate a process size
        if (!memory[i].allocated && memory[i].size >= size)
        {
            // find beginning address for allocation
            int start = memory[i].start;

            // create a new block for the remaining memory if it is too large to accommodate
            if (memory[i].size > size)
            {
                memory[i + 1].start = start + size;
                memory[i + 1].size = memory[i].size - size;
                memory[i + 1].allocated = False;
                strcpy(memory[i + 1].processName, "");
            }

            // memory block is exact size of our data, no need to create new block
            memory[i].size = size;
            memory[i].allocated = True;
            strcpy(memory[i].processName, processName);

            printf("Successfully allocated %d to process %s\n", size, processName);
            return True;
        }
    }

    // no memory available
    printf("No hole of sufficient space\n");
    return False;
}

void printMemoryStatus()
{
    int allocatedMemory = 0;
    int freeMemory = 0;
    int i = 0;

    printf("Partitions [Allocated memory= ");

    // calculate total allocated memory and print allocated partitions
    while (i < memorySize && memory[i].size > 0)
    {
        // allocated block => add it to our running total
        if (memory[i].allocated)
        {
            allocatedMemory += memory[i].size;
        }
        i++;
    }
    // complete the allocated output
    printf("%d]:\n", allocatedMemory);

    // print allocated partitions
    i = 0;
    while (i < memorySize && memory[i].size > 0)
    {
        if (memory[i].allocated)
        {
            // add to processes that are allocated to output
            printf("Address [%d:%d] Process %s\n",
                   memory[i].start,
                   memory[i].start + memory[i].size - 1,
                   memory[i].processName);
        }
        i++;
    }

    printf("Holes [Free memory= ");

    // calculate total free memory
    i = 0;
    while (i < memorySize && memory[i].size > 0)
    {
        // free-memory => add it to the free running total
        if (!memory[i].allocated)
        {
            freeMemory += memory[i].size;
        }
        i++;
    }
    printf("%d]:\n", freeMemory);

    // print free partitions (holes)
    i = 0;
    while (i < memorySize && memory[i].size > 0)
    {
        if (!memory[i].allocated)
        {
            // add to holes that are not allocated to output
            printf("Address [%d:%d] len = %d\n",
                   memory[i].start,
                   memory[i].start + memory[i].size - 1,
                   memory[i].size);
        }
        i++;
    }
}

void compactMemory()
{
    int compactedIndex = 0;
    int i;

    // move all allocated blocks to the beginning of memory
    for (i = 0; i < memorySize && memory[i].size > 0; i++)
    {
        // allocated memory block => move it to the start
        if (memory[i].allocated)
        {
            if (i != compactedIndex)
            {
                memory[compactedIndex] = memory[i];
                memory[compactedIndex].start = compactedIndex > 0 ? memory[compactedIndex - 1].start + memory[compactedIndex - 1].size : 0;
            }
            compactedIndex++;
        }
    }

    // decrement from total free memory by taking away memory that is already used
    int totalFreeMemory = memorySize;

    for (i = 0; i < compactedIndex; i++)
    {
        totalFreeMemory -= memory[i].size;
    }

    // create block of memory at the end if memory is free there
    if (totalFreeMemory > 0)
    {
        // check if the location to compact memory from is not at the start of the entire memory block
        memory[compactedIndex].start = compactedIndex > 0 ? memory[compactedIndex - 1].start + memory[compactedIndex - 1].size : 0;

        // update size
        memory[compactedIndex].size = totalFreeMemory;

        // update allocation status
        memory[compactedIndex].allocated = False;

        // unassign process ID to the compacted process
        strcpy(memory[compactedIndex].processName, "");
        compactedIndex++;
    }

    // clear any remaining entries in the memory array
    for (i = compactedIndex; i < memorySize; i++)
    {
        memory[i].start = 0;
        memory[i].size = 0;
        memory[i].allocated = False;
        strcpy(memory[i].processName, "");
    }

    // successful compacte
    printf("Memory compaction completed.\n");
    return;
}
