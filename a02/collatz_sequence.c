#include <stdio.h>    // accesses standard input/output functions
#include <stdlib.h>   // accesses memory allocation and exit functions
#include <string.h>   // accesses string manipulation functions
#include <sys/mman.h> // accesses memory management functions

// function prototypes
int* createCollatzSequence(int);
void printCollatzSequence(int *, int);
void readANumberFromTextFile();
int* createSharedMemoryObject()

// global variables
int* NUMBERS;
int SIZE; // used only by createCollatzSequence
char* FILENAME = "start_numbers.txt";

int main()
{
     // create shared memory object 
     int *sharedMemoryObject = createSharedMemoryObject();

     // open file and read a number from it 
     char *file = FILENAME;
     FILE *fp = fopen(file, "r");

     if (fp == NULL) {
          printf("File reading error. Could not read {%s}\n", file);
          exit(1);
     }

     int currentNumber;
     int num; 
     while (fscanf(fp, "%d", &num) != EOF) {
          currentNumber = num;
          printf("Parent Process: The positive integer read from file is {%d}\n", currentNumber);

          // create collatz sequence for the current number
          int *seq = createCollatzSequence(currentNumber);

          // save the sequence to the shared memory object
          for (int i = 0; i < SIZE; i++) {
               sharedMemoryObject[i] = seq[i];
          }

          // create child process that will print the collatz sequence
          pid_t pid = fork();
          if (pid == 0) {
               printCollatzSequence(seq, SIZE);
               exit(0);
          }
     }
}

int* createCollatzSequence(int num) {
     int *sequence = (int*)malloc(1000 * sizeof(int));
     int i = 0; 

     while (num != 1) {
          sequence[i] = num; 
          i++;
          SIZE = i;
          // even 
          if (num % 2 == 0) {
               num = num/2;
          // odd
          } else {
               num = 3*num + 1;
          }
     }
     sequence[i] = num; // assign 1 to the last element
     return sequence;
}

void printCollatzSequence(int *seq, int size) {
     print("Child Process: The Collatz sequence is: ");
     for (int i = 0; i <= size; i++) {
          if (i == size) {
               printf("%d\n", seq[i]);
          } else {
               printf("%d, \n", seq[i]);
          }
     }
}

void readANumberFromTextFile() {
     char *file = FILENAME;
     FILE *fp = fopen(file, "r");

     if (fp == NULL) {
          printf("File reading error. Could not read {%s}\n", file);
          exit(1);
     }

     int num; 
     int i = 0; 
     while (fscanf(fp, "%d", &num) != EOF) {
          NUMBERS[i] = num;
          i++;
     }

     fclose(fp);
}

int *createSharedMemoryObject() {
     // integer file descriptor for shared-memory object
     int shm_fd = shm_open("collatz_sequence", O_CREAT | O_RDWR, 0666);

     // configure the size of the shared-memory object in bytes
     ftruncate(shm_fd, sizeOf(int) * SIZE);

     // memory map the shared-memory object
     int *ptr = mmap(0, SIZE * sizeof(int), PROT_WRITE, MAP_SHARED, shm_fd, 0);

     return ptr;
}