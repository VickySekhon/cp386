#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1048576 // Default memory size
#define COMMAND_LINE_PARAMETERS 2 // one command-line that specifies the size of allocated memory in bytes
#define BUFFER 100 // general buffer

typedef enum {
    False,
    True
} Boolean;

typedef struct {
     int start; // location where memory memory starts 
     int size; // size of memory
     char processName[20]; // unique identifier for a memory memory
     Boolean allocated; // tracks whether or not memory allocation was successful
} MemoryBlock;

// global memory block
MemoryBlock memory[MAX];
int memorySize = MAX;

// function prototypes
void initializeMemory();
Boolean exitRequest(char *);
Boolean allocationRequest(char *);
Boolean releaseRequest(char *);
Boolean compactionRequest(char *);
Boolean statusRequest(char *);

// Print memory status
void printMemoryStatus()
{
 int allocatedMemory = 0;
    int freeMemory = 0;
    int i = 0;

    printf("Partitions [Allocated memory= ");

    // First pass: calculate total allocated memory and print allocated partitions
    while (i < MAX && memory[i].size > 0)
    {
        if (memory[i].allocated)
        {
            allocatedMemory += memory[i].size;
        }
        i++;
    }
    printf("%d]:\n", allocatedMemory);

    // Second pass: print allocated partitions
    i = 0;
    while (i < MAX && memory[i].size > 0)
    {
        if (memory[i].allocated)
        {
            printf("Address [%d:%d] Process %s\n", 
                   memory[i].start, 
                   memory[i].start + memory[i].size - 1, 
                   memory[i].processName);
        }
        i++;
    }

    printf("Holes [Free memory= ");

    // Third pass: calculate total free memory
    i = 0;
    while (i < MAX && memory[i].size > 0)
    {
        if (!memory[i].allocated)
        {
            freeMemory += memory[i].size;
        }
        i++;
    }
    printf("%d]:\n", freeMemory);

    // Fourth pass: print free partitions (holes)
    i = 0;
    while (i < MAX && memory[i].size > 0)
    {
        if (!memory[i].allocated)
        {
            printf("Address [%d:%d] len = %d\n", 
                   memory[i].start, 
                   memory[i].start + memory[i].size - 1, 
                   memory[i].size);
        }
        i++;
    }
}

// First fit memory allocation
Boolean requestMemory(char *processName, int size)
{
    for (int i = 0; i < memorySize; i++)
    {
        if (!memory[i].allocated && memory[i].size >= size)
        {
            // Allocate memory
            int start = memory[i].start;
            if (memory[i].size > size)
            {
                // Create a new block for the remaining memory
                memory[i + 1].start = start + size;
                memory[i + 1].size = memory[i].size - size;
                memory[i + 1].allocated = False;
                strcpy(memory[i + 1].processName, "");
            }
            memory[i].size = size;
            memory[i].allocated = True;
            strcpy(memory[i].processName, processName);

            printf("Successfully allocated %d to process %s\n", size, processName);
            return True;
        }
    }

    printf("No hole of sufficient space\n");
    return False;
}

// Release memory
void releaseMemory(char *processName)
{
    for (int i = 0; i < MAX; i++)
    {
        if (memory[i].allocated && strcmp(memory[i].processName, processName) == 0)
        {
            memory[i].allocated = False;
            strcpy(memory[i].processName, "");
            printf("Releasing memory for process %s\n", processName);
            return;
        }
    }
    printf("Process not found\n");
}

// Compact memory
void compactMemory()
{
    // TODO: Implement memory compaction
}

// Main function
int main(int argc, char *argv[])
{
    if (argc != COMMAND_LINE_PARAMETERS)
    {
        printf("Insufficient command line parameters passed. Cannot proceed.\n");
        exit(1); // exit failure
    }

    // get memory size and convert it to int
    memorySize = atoi(argv[1]);
    initializeMemory();

    char userInput[BUFFER];

    while (1)
    {
        printf("allocator>");
        fgets(userInput, sizeof(userInput), stdin); // get user input

        if (exitRequest(userInput))
        {
            break;
        }
        else if (allocationRequest(userInput))
        {
            char processName[BUFFER];
            int size;
            char algorithm[2];

            // get process ID, size of memory memory, and algorithm of memory block to allocate
            sscanf(userInput, "RQ %s %d %s", processName, &size, algorithm);

            // selected "First Fit (F)" algorithm
            if (strcmp(algorithm, "F") == 0)
            {
                requestMemory(processName, size);
            }
            else
            {
                printf("Unknown allocation algorithm\n");
            }
        }
        else if (strncmp(userInput, "RL", 2) == 0)
        {
            char processName[20];
            sscanf(userInput, "RL %s", processName);
            releaseMemory(processName);
        }
        else if (strcmp(userInput, "C\n") == 0)
        {
            compactMemory();
        }
        else if (strcmp(userInput, "Status\n") == 0)
        {
            printMemoryStatus();
        } else {
            printf("Unknown command entered. Please try again.\n");
        }
    }

    return 0;
}

// Initialize memory with one large free block
void initializeMemory()
{
    memory[0].start = 0;
    memory[0].size = memorySize;
    memory[0].allocated = False;
    strcpy(memory[0].processName, "");
}

// function implementations
Boolean exitRequest(char *userRequest) {
     return strcmp(userRequest, "Exit\n") == 0;
}

Boolean allocationRequest(char *userRequest) {
     return strncmp(userRequest, "RQ", 2) == 0;
}

Boolean releaseRequest(char *userRequest) {
     return strncmp(userRequest, "RL", 2) == 0;
}

Boolean compactionRequest(char *userRequest) {
     return strcmp(userRequest, "C\n") == 0;
}

Boolean statusRequest(char *userRequest) {
     return strcmp(userRequest, "Status\n") == 0;
}

Boolean validAlgorithmRequest(char *algorithm) {
     return strcmp(algorithm, "F") == 0; // this program only supports the first fit contiguous allocation method
}
