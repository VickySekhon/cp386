// libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1048576 // Default memory size

// constants
#define COMMAND_LINE_PARAMETERS 2 // one command-line that specifies the size of allocated memory in bytes
#define TOTAL_PROCESSES 100 // total process IDs = 100
#define TOTAL_COMMAND_LENGTH 100 // maximum chars needed for a command = 6 ("Status")
#define BUFFER 50 // generic buffer for strings

typedef enum {
     True, 
     False
} Boolean;

typedef struct {
     int start; // location where memory memory starts 
     int size; // size of memory
     char processID[TOTAL_PROCESSES]; // unique identifier for a memory memory
     Boolean allocated; // tracks whether or not memory allocation was successful
} MemoryBlock;

// global memory block
MemoryBlock memory[MAX];
int memorySize = MAX;

// function prototypes

// user requests
Boolean exitRequest(char[]);
Boolean allocationRequest(char[]);
Boolean releaseRequest(char[]);
Boolean compactionRequest(char[]);
Boolean statusRequest(char[]);

// checks
Boolean validAlgorithmRequest(char *);

// memory management
void initializeMemory();

// memory allocation
Boolean handleAllocationRequest(char *, int);

int main(int argc, char const *argv[])
{
     if (argc != COMMAND_LINE_PARAMETERS) {
          printf("Insufficient command line parameters passed. Cannot proceed.\n");
          exit(1); // exit failure
     }

     // get memory size and convert it to int
     memorySize = atoi(argv[1]);
     initializeMemory();
     
     char userInput[TOTAL_COMMAND_LENGTH]; 

     while (1) {
          printf("allocator>");
          fgets(userInput, sizeof(userInput), stdin); // get user input
          
          if (exitRequest(userInput)) {
               break;
          } else if (allocationRequest(userInput)) {
               char processID[BUFFER];
               int processSize;
               char algorithm[2];

               // get process ID, size of memory memory, and algorithm of memory block to allocate
               sscanf(userInput, "RQ %s %d %s", processID, &processSize, algorithm);

               // selected "First Fit (F)" algorithm
               if (strcmp(algorithm, "F") == 0) {
                    handleAllocationRequest(processID, processSize);
               } else {
                    printf("Invalid allocation algorithm. Check input to ensure selected algorithm is First Fit (F).\n");
               }
          } else if (releaseRequest(userInput)) {
               printf("Release request made.\n");
          } else if (compactionRequest(userInput)) {
               printf("Compaction request made.\n");
          } else if (statusRequest(userInput)) {
               printf("Status request made.\n");
          }
     }
     return 0;
}


// function implementations
Boolean exitRequest(char userRequest[]) {
     return strcmp(userRequest, "Exit\n") == 0;
}

Boolean allocationRequest(char userRequest[]) {
     return strncmp(userRequest, "RQ", 2) == 0;
}

Boolean releaseRequest(char userRequest[]) {
     return strncmp(userRequest, "RL", 2) == 0;
}

Boolean compactionRequest(char userRequest[]) {
     return strcmp(userRequest, "C\n") == 0;
}

Boolean statusRequest(char userRequest[]) {
     return strcmp(userRequest, "Status\n") == 0;
}

Boolean validAlgorithmRequest(char *algorithm) {
     return strcmp(algorithm, "F") == 0; // this program only supports the first fit contiguous allocation method
}

void initializeMemory() {
     memory[0].start = 0;
     memory[0].size = memorySize;
     memory[0].allocated = False; // memory is yet to be occupied
     strcpy(memory[0].processID, ""); // total block should have no name (can't release block)
}

Boolean handleAllocationRequest(char *processID, int processSize) {
    for (int i = 0; i < memorySize; i++)
    {
        if (!memory[i].allocated && memory[i].size >= processSize)
        {
          // Allocate memory
          int start = memory[i].start;
          if (memory[i].size > processSize)
          {
               // Create a new block for the remaining memory
               memory[i + 1].start = start + processSize;
               memory[i + 1].size = memory[i].size - processSize;
               memory[i + 1].allocated = False;
               strcpy(memory[i + 1].processID, "");
          }
          memory[i].size = processSize;
          memory[i].allocated = True;
          strcpy(memory[i].processID, processID);

          printf("Successfully allocated %d to process %s\n", processSize, processID);
          return True;
        }
    }

    printf("No hole of sufficient size\n");
    return False;
}
