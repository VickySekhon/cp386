#include <stdio.h>    // accesses standard input/output functions
#include <stdlib.h>   // accesses memory allocation and exit functions
#include <unistd.h>   // accesses POSIX operating system API
#include <sys/mman.h> // accesses memory management functions
#include <sys/stat.h> // accesses file information functions
#include <fcntl.h>    // accesses file control options
#include <sys/wait.h> // accesses waitpid function

// definitions
#define SHM_NAME "/collatz_shm"
#define MAX_SEQUENCE_LENGTH 1000 // maximum length of a generated collatz sequence
#define ARRAY_SIZE 100           // maximum number of start numbers
#define FILE_NAME "start_numbers.txt"

// function prototypes
void createCollatzSequence(int, int *);

int *createSharedMemoryObject();

int main()
{
     FILE *fp = fopen("start_numbers.txt", "r");
     if (fp == NULL)
     {
          printf("Error opening file\n");
          return 1;
     }

     // array to store start numbers
     int start_numbers[ARRAY_SIZE];
     int index = 0;
     int num;

     // populate start_numbers with every number from the file start_numbers.txt
     while (fscanf(fp, "%d", &num) == 1)
     {
          start_numbers[index++] = num;
     }

     // close the file because we are finished reading numbers from it
     fclose(fp);

     for (int i = 0; i < index; i++)
     {
          // shared memory object is created and is waiting to be populated with collatz sequence
          int *sequence = createSharedMemoryObject();

          // generate collatz sequence in shared memory object
          createCollatzSequence(start_numbers[i], sequence);

          // sequence is a shared-mem obj that has the collatz sequence for a number

          // create child process
          pid_t pid = fork();

          // child process was not created successfully
          if (pid == -1)
          {
               printf("Error forking process\n");
               exit(1);
               // child process: print collatz sequence
          }
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
               // parent process: wait for child process to finish executing
          }
          else
          {
               printf("Parent Process: The positive integer read from file is %d\n", start_numbers[i]);
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
          exit(1);
     }

     // shared memory object will hold the generated collatz sequence for a start_number...
     // .. therefore set the size of the shared memory object to hold the maximum possible length of a collatz sequence
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
     while (number != 1)
     {
          sequence[i] = number;
          // even: n = n/2
          if (number % 2 == 0)
          {
               number = number / 2;
               // odd: n = 3n + 1
          }
          else
          {
               number = 3 * number + 1;
          }
          i++;
     }
     sequence[i] = 1;
}