#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1048576 // Default memory size

typedef enum
{
    FALSE,
    TRUE
} bool;

typedef struct
{
    int start;
    int size;
    bool allocated;
    char processName[20];
} MemoryBlock;

MemoryBlock memory[MAX];
int memorySize = MAX;

// Initialize memory with one large free block
void initializeMemory()
{
    memory[0].start = 0;
    memory[0].size = memorySize;
    memory[0].allocated = FALSE;
    strcpy(memory[0].processName, "");
}

// Print memory status
void printMemoryStatus()
{
    printf("Partitions [Allocated memory= %d]:\n", 0); // Placeholder
    printf("Holes [Free memory= %d]:\n", 0);           // Placeholder
}

// First fit memory allocation
bool requestMemory(char *processName, int size)
{
    for (int i = 0; i < MAX; i++)
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
                memory[i + 1].allocated = FALSE;
                strcpy(memory[i + 1].processName, "");
            }
            memory[i].size = size;
            memory[i].allocated = TRUE;
            strcpy(memory[i].processName, processName);

            printf("Successfully allocated %d to process %s\n", size, processName);
            return TRUE;
        }
    }

    printf("No hole of sufficient size\n");
    return FALSE;
}

// Release memory
void releaseMemory(char *processName)
{
    for (int i = 0; i < MAX; i++)
    {
        if (memory[i].allocated && strcmp(memory[i].processName, processName) == 0)
        {
            memory[i].allocated = FALSE;
            strcpy(memory[i].processName, "");
            printf("Successfully released memory for process %s\n", processName);
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
    if (argc != 2)
    {
        printf("Usage: %s <memory size>\n", argv[0]);
        return 1;
    }

    memorySize = atoi(argv[1]);
    initializeMemory();

    char command[100];
    while (1)
    {
        printf("allocator>");
        fgets(command, sizeof(command), stdin);

        if (strncmp(command, "RQ", 2) == 0)
        {
            char processName[20];
            int size;
            char algorithm[2];
            sscanf(command, "RQ %s %d %s", processName, &size, algorithm);

            if (strcmp(algorithm, "B") == 0)
            {
                requestMemory(processName, size);
            }
            else
            {
                printf("Unknown allocation algorithm\n");
            }
        }
        else if (strncmp(command, "RL", 2) == 0)
        {
            char processName[20];
            sscanf(command, "RL %s", processName);
            releaseMemory(processName);
        }
        else if (strcmp(command, "C\n") == 0)
        {
            compactMemory();
        }
        else if (strcmp(command, "Status\n") == 0)
        {
            printMemoryStatus();
        }
        else if (strcmp(command, "Exit\n") == 0)
        {
            break;
        }
        else
        {
            printf("Unknown command\n");
        }
    }

    return 0;
}
