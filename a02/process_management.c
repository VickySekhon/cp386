

#include <stdio.h>    // accesses standard input/output functions
#include <stdlib.h>   // accesses memory allocation and exit functions
#include <unistd.h>   // accesses POSIX operating system API
#include <string.h>   // accesses string manipulation functions
#include <sys/mman.h> // accesses memory management functions
#include <sys/ipc.h>  // accesses inter-process communication functions
#include <sys/shm.h>  // accesses shared memory functions
#include <fcntl.h>    // accesses file control options
#include <sys/wait.h> // accesses waitpid function

// define constants

#define SHM_NAME "/process_management_shm" // name of shared memory object
#define SHM_SIZE 1024 // size of shared memory object
#define COMMAND_LINE_ARGUMENTS 2 // number of command line arguments
#define BUFFER_SIZE 4096 // size of buffer

// function prototypes

void writeOutput(char *, char *);

int main(int argc, char *argv[])
{
     // check if there an argument passed to read that specific file name
     if (argc != COMMAND_LINE_ARGUMENTS)
     {
          printf("Insufficient parameters passed.\n");
          return 1; // abnormal termination
     }

     // open file to read commands from
     int file = open(argv[1], O_RDONLY);
     if (file == -1)
     {
          printf("Error opening file\n");
          return 1;
     }

     char *sharedMemoryObject = createSharedMemoryObject();

     // create pipe that goes from child to parent process
     int pipefd[2];

     // check status of creating pipe
     if (pipe(pipefd) == -1)
     {
          printf("Pipe failed\n");
          return 1;
     }

     // parent creates a child process to read the contents of the file name
     pid_t pid = fork();

     // fork failed
     if (pid < 0)
     {
          printf("Fork failed\n");
          return 1;
     }

     // child process
     if (pid == 0)
     {
          char buffer[BUFFER_SIZE];
          int length = 0;

          // read the contents of the file and write it to the shared memory object
          while ((length = read(file, buffer, BUFFER_SIZE)) > 0)
          {
               strncpy(sharedMemoryObject, buffer, SHM_SIZE);
               sharedMemoryObject[length] = '\0';
          }

          close(file);
          exit(0);
     } 
     // parent process
     else if (pid > 0) {
          // parent waits for child process to finish
          wait(NULL);

          // create another child process to execute commands that are written into the shared memory object
          pid_t pid2 = fork();

          // fork failed
          if (pid2 < 0)
          {
               printf("Fork failed\n");
               return 1;
          }    

          // child process
          if (pid2 == 0) 
          {
               dup2(pipefd[1], STDOUT_FILENO);
               close(pipefd[0]);
               close(pipefd[1]);

               // execute the commands that are written into the shared memory object
               execvp(args[0], args);

               exit(0);
          }
          // parent process
          else if (pid2 > 0)
          {
               wait(NULL);

               close(pipefd[1]);

               char buffer[BUFFER_SIZE];
               int length = 0;

               // read the output of the commands from the pipe
               while ((length = read(pipefd[0], buffer, BUFFER_SIZE)) > 0)
               {
                    writeOutput(commands, buffer);
               }
          }
          close(file);
     }
     return 0;
}

int *createSharedMemoryObject()
{
     char *obj;

     // open new or existing shared memory object
     int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);

     // check status of opening or creating shared memory object
     if (shm_fd == -1)
     {
          printf("Error creating shared memory\n");
          exit(1); // unsuccessful creation of shared memory object, exit program with error code 1
     }

     // map shared memory object to the address space of the calling process
     obj = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

     // check status of mapping shared memory object
     if (obj == MAP_FAILED)
     {
          printf("Error mapping shared memory\n");
          exit(1);
     }

     // set the size of the shared memory object
     if (ftruncate(shm_fd, SHM_SIZE) == -1) {
          printf("Error setting the size of the shared memory object\n");
          exit(1);
     }

     return obj;
}

void writeOutput(char *command, char *output)
{
     FILE *fp;
     fp = fopen("output.txt", "a");
     fprintf(fp, "The output of: %s : is\n", command);
     fprintf(fp, ">>>>>>>>>>>>>>>\n%s<<<<<<<<<<<<<<<\n", output);
     fclose(fp);
}