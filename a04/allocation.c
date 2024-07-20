/*
 -------------------------------------
 File: allocation.c

 Multiple resource allocation and deadlock avoidance-related
 programming concepts and contiguous memory allocation.
 -------------------------------------
 Author:  Vicky Sekhon, Yafet Tegbaru
 ID:      169024498, 210480140
 Email:   sekh4498@mylaurier.ca, tegb0140@mylaurier.ca
 Version  2024-07-20
 -------------------------------------
 */

#include <stdio.h>  // Include the standard input-output library for basic functions
#include <stdlib.h> // Include the standard library for functions like malloc and atoi
#include <string.h> // Include the string library for functions like strcmp

// Define a struct to represent a block of memory allocated to a process
typedef struct
{
     int start;      // Starting address of the memory block
     int size;       // Size of the memory block
     int process_id; // ID of the process that owns this block
} MemoryBlock;

// Define a struct to represent a hole, which is a block of free memory
typedef struct
{
     int start; // Starting address of the free memory block
     int size;  // Size of the free memory block
} MemoryHole;

// Declare a pointer to an array of MemoryHole structs to keep track of free memory
MemoryHole *holes;
// Declare a variable to keep track of the number of free memory holes
int hole_count = 0;

// Declare a pointer to an array of MemoryBlock structs to keep track of allocated memory
MemoryBlock *blocks;
// Declare a variable to keep track of the number of allocated memory blocks
int block_count = 0;

// Function to request memory for a process
void request_memory(int process_id, int size, char strategy)
{
     int i, best_index = -1, worst_index = -1;

     // Search for a suitable hole based on the chosen strategy
     for (i = 0; i < hole_count; i++)
     {
          if (holes[i].size >= size)
          {
               if (strategy == 'F')
               {
                    best_index = i;
                    break;
               }
               else if (strategy == 'B' && (best_index == -1 || holes[i].size < holes[best_index].size))
               {
                    best_index = i;
               }
               else if (strategy == 'W' && (worst_index == -1 || holes[i].size > holes[worst_index].size))
               {
                    worst_index = i;
               }
          }
     }

     // Determine which index to use for allocation
     int index = (strategy == 'W') ? worst_index : best_index;

     // If a suitable hole is found, allocate memory
     if (index != -1)
     {
          // Allocate memory block
          blocks[block_count].start = holes[index].start;
          blocks[block_count].size = size;
          blocks[block_count].process_id = process_id;
          block_count++;

          // Update the hole's start address and size
          holes[index].start += size;
          holes[index].size -= size;

          // Remove the hole if its size is zero
          if (holes[index].size == 0)
          {
               for (i = index; i < hole_count - 1; i++)
               {
                    holes[i] = holes[i + 1];
               }
               hole_count--;
          }

          printf("Successfully allocated %d to process %d\n", size, process_id);
     }
     else
     {
          printf("Error: Not enough memory to allocate %d to process %d\n", size, process_id);
     }
}

// Function to release memory previously allocated to a process
void release_memory(int process_id)
{
     int i, j;

     // Search for the memory block belonging to the process
     for (i = 0; i < block_count; i++)
     {
          if (blocks[i].process_id == process_id)
          {
               // Create a new hole where the block was
               holes[hole_count].start = blocks[i].start;
               holes[hole_count].size = blocks[i].size;
               hole_count++;

               // Shift remaining blocks to fill the gap
               for (j = i; j < block_count - 1; j++)
               {
                    blocks[j] = blocks[j + 1];
               }
               block_count--;

               // Merge adjacent holes
               for (i = 0; i < hole_count - 1; i++)
               {
                    if (holes[i].start + holes[i].size == holes[i + 1].start)
                    {
                         holes[i].size += holes[i + 1].size;
                         for (j = i + 1; j < hole_count - 1; j++)
                         {
                              holes[j] = holes[j + 1];
                         }
                         hole_count--;
                         i--;
                    }
               }

               printf("Successfully released memory for process %d\n", process_id);
               return;
          }
     }
     printf("Error: Process %d not found\n", process_id);
}

// Function to compact memory by combining all free holes into one large hole
void compact_memory()
{
     int i, total_free = 0;

     // Calculate total free memory
     for (i = 0; i < hole_count; i++)
     {
          total_free += holes[i].size;
     }

     // Reset the holes array
     hole_count = 1;
     holes[0].start = 0;
     holes[0].size = total_free;

     // Reallocate memory blocks to the beginning of the memory
     int current_address = total_free;
     for (i = block_count - 1; i >= 0; i--)
     {
          blocks[i].start = current_address;
          current_address += blocks[i].size;
     }

     printf("Compaction process is successful\n");
}

// Function to report the current status of memory allocation
void report_status()
{
     int i;

     // Print allocated memory blocks
     printf("Partitions [Allocated memory= %d]:\n", block_count);
     for (i = 0; i < block_count; i++)
     {
          printf("Address [%d:%d] Process %d\n", blocks[i].start, blocks[i].start + blocks[i].size - 1, blocks[i].process_id);
     }

     // Print free memory holes
     printf("Holes [Free memory= %d]:\n", hole_count);
     for (i = 0; i < hole_count; i++)
     {
          printf("Address [%d:%d] len = %d\n", holes[i].start, holes[i].start + holes[i].size - 1, holes[i].size);
     }
}

// Main function where the program starts execution
int main(int argc, char *argv[])
{
     // Check if the user provided the initial memory size as a command line argument
     if (argc < 2)
     {
          // If not, print an error message and exit the program
          fprintf(stderr, "Usage: %s <initial memory size>\n", argv[0]);
          return 1;
     }

     // Convert the initial memory size argument from a string to an integer
     int MAX = atoi(argv[1]);

     // Allocate memory for the holes array to store information about free memory blocks
     holes = (MemoryHole *)malloc(MAX * sizeof(MemoryHole));
     // Allocate memory for the blocks array to store information about allocated memory blocks
     blocks = (MemoryBlock *)malloc(MAX * sizeof(MemoryBlock));

     // Initialize the first hole to cover the entire memory range
     holes[0].start = 0;
     holes[0].size = MAX;
     // Set the initial number of holes to 1
     hole_count = 1;

     // Start an infinite loop to process user commands
     while (1)
     {
          // Declare a string to store the user's command
          char command[10];
          // Read the command from the user
          scanf("%s", command);

          // If the command is "RQ", request memory for a process
          if (strcmp(command, "RQ") == 0)
          {
               int process_id, size;
               char strategy;
               // Read the process ID, memory size, and allocation strategy from the user
               scanf("%d %d %c", &process_id, &size, &strategy);
               // Call the request_memory function to allocate memory
               request_memory(process_id, size, strategy);
          }
          // If the command is "RL", release memory for a process
          else if (strcmp(command, "RL") == 0)
          {
               int process_id;
               // Read the process ID from the user
               scanf("%d", &process_id);
               // Call the release_memory function to free the memory
               release_memory(process_id);
          }
          // If the command is "C", compact the memory
          else if (strcmp(command, "C") == 0)
          {
               // Call the compact_memory function to merge free holes
               compact_memory();
          }
          // If the command is "Status", report the current status of memory allocation
          else if (strcmp(command, "Status") == 0)
          {
               // Call the report_status function to print memory status
               report_status();
          }
          // If the command is "Exit", break out of the loop and end the program
          else if (strcmp(command, "Exit") == 0)
          {
               break;
          }
     }

     // Free the memory allocated for the holes and blocks arrays before exiting the program
     free(holes);
     free(blocks);
     return 0;
}
