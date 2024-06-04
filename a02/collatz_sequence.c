/*
 -------------------------------------
 File: collatz_sequence.c
 Project: sekh4498_a02, tegb0140_a02

 This file contains a program that uses a parent process
 to create and execute a child process which prints the collatz 
 sequence for a number read from the start_number.txt file.
 -------------------------------------
 Authors:  Vicky Sekhon, Yafet Tegbaru
 IDs:      169024498, 210480140
 Emails:   sekh4498@mylaurier.ca, tegb0140@mylaurier.ca
 Version   2024-06-04
 -------------------------------------
 */

#include <stdio.h>    // accesses standard input/output functions
#include <stdlib.h>   // accesses memory allocation and exit functions
#include <unistd.h>   // accesses POSIX operating system API
#include <sys/mman.h> // accesses memory management functions
#include <sys/stat.h> // accesses file information functions
#include <fcntl.h>    // accesses file control options
#include <sys/wait.h> // accesses waitpid function

// define constants 

#define SHM_NAME "/collatz_shm" // name of shared memory object
#define MAX_SEQUENCE_LENGTH 1000 // maximum length of a generated collatz sequence
#define ARRAY_SIZE 100 // maximum number of start numbers to generate collatz sequence for
#define FILE_NAME "start_numbers.txt" // name of file to read start numbers from

// function prototypes

void createCollatzSequence(int, int *);
int *createSharedMemoryObject();

int main()
{    
     // open file to read start numbers from
     FILE *fp = fopen("start_numbers.txt", "r");

     // update status of opening file
     if (fp == NULL)
     {
          printf("Error opening file\n");
          return 1;
     }

     // array to store start numbers
     int startNumbers[ARRAY_SIZE];
     
     int index = 0;

     int num;

     // populate startNumbers array with every number from the file start_numbers.txt
     while (fscanf(fp, "%d", &num) == 1)
     {
          startNumbers[index++] = num;
     }

     // close the start-numbers.txt file because we are finished reading numbers from it
     fclose(fp);

     // iterate through startNumbers array and generate collatz sequence for each number
     for (int i = 0; i < index; i++)
     {
          // shared memory object is created and is waiting to be populated with collatz sequence
          int *sequence = createSharedMemoryObject();

          // generate collatz sequence for number "i" and store it inside the shared memory object
          createCollatzSequence(startNumbers[i], sequence);

          // create a child process
          pid_t pid = fork();

          // child process was not created successfully
          if (pid == -1)
          {
               printf("Error forking process\n");
               exit(1);
          }
          // this is the child process => print collatz sequence
          else if (pid == 0)
          {
               printf("Child Process: The generated collatz sequence is ");
               for (int j = 0; j < MAX_SEQUENCE_LENGTH; j++)
               {
                    if (sequence[j] == 1)
                    {
                         break;
                    }
                    printf("%d ", sequence[j]);
               }
               printf("1\n");
               return 0;
          }
          // this is the parent process => wait for child process to finish executing
          else
          {
               printf("Parent Process: The positive integer read from file is %d\n", startNumbers[i]);
               wait(NULL);
          }
     }
}

int *createSharedMemoryObject()
{
     // open new or existing shared memory object
     int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);

     // check status of opening or creating shared memory object
     if (shm_fd == -1)
     {
          printf("Error creating shared memory\n");
          exit(1); // unsuccessful creation of shared memory object, exit program with error code 1
     }

     // set the size of the shared memory object to account for the maximum possible length of a collatz sequence (1000 integers)
     ftruncate(shm_fd, MAX_SEQUENCE_LENGTH * sizeof(int));

     // map shared memory object to the address space of the calling process
     int *sequence = mmap(NULL, MAX_SEQUENCE_LENGTH * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

     // check status of mapping shared memory object
     if (sequence == MAP_FAILED)
     {
          printf("Error mapping shared memory\n");
          exit(1); 
     }

     return sequence;
}

void createCollatzSequence(int number, int *sequence)
{
     int i = 0;
     
     // generate collatz sequence until we reach 1 (end of sequence)
     while (number != 1)
     {    
          // update the shared memory object array with the current number in the sequence
          sequence[i] = number;

          // STATEMENT #1: number is even, divide it by 2
          if (number % 2 == 0)
          {
               number = number / 2;
          }

          // STATEMENT #2: number is odd, triple it and add 1
          else
          {
               number = 3 * number + 1;
          }

          // increment the index of the shared memory object array
          i++;
     }

     // end of collatz sequence is approached => update the last element of the shared memory object array with 1
     sequence[i] = 1;
}