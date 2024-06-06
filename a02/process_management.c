/*
 -------------------------------------
 File: process_management.c
 Project: sekh4498_a02, tegb0140_a02

 This file contains a program that uses multiple parent, child
 processes and a grandchild process which:

 1) write POSIX commands to a shared memory object
 2) read from a shared memory object to tokenize commands one-by-one
 3) use a pipe to communicate between grandchild-child-parent processes
 4) create dynamic processes to execute each POSIX command independently
 5) writes the associated command and it's output to a file marked 'output.txt'
 -------------------------------------
 Authors:  Vicky Sekhon, Yafet Tegbaru
 IDs:      169024498, 210480140
 Emails:   sekh4498@mylaurier.ca, tegb0140@mylaurier.ca
 Version   2024-06-06
 -------------------------------------
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// define constants

#define SHM_NAME "/process_management_shm" // name of shared memory object
#define COMMAND_LINE_ARGUMENTS 2           // number of command line arguments
#define BUFFER_SIZE 1024                   // size of buffer

void writeOutput(char *command, char *output)
{
     FILE *fp;
     fp = fopen("output.txt", "a");
     fprintf(fp, "The output of: %s : is\n", command);
     fprintf(fp, ">>>>>>>>>>>>>>>\n%s<<<<<<<<<<<<<<<\n", output);
     fclose(fp);
}

char *createSharedMemoryObject()
{
     // open new or existing shared memory object
     int fileDescriptor = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);

     // check status of opening or creating shared memory object
     if (fileDescriptor == -1)
     {
          printf("Error creating shared memory\n");
          exit(1); // unsuccessful creation of shared memory object, exit program with error code
     }

     // set the size of the shared memory object
     if (ftruncate(fileDescriptor, BUFFER_SIZE) == -1)
     {
          printf("Error setting the size of the shared memory object\n");
          exit(1);
     }

     // map shared memory object to the address space of the calling process
     char *sharedMemoryObject = mmap(NULL, BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);

     // check status of mapping shared memory object
     if (sharedMemoryObject == MAP_FAILED)
     {
          printf("Error mapping shared memory\n");
          exit(1);
     }

     return sharedMemoryObject;
}

int main(int argc, char *argv[])
{
     // check for input file as a command line argument
     if (argc != COMMAND_LINE_ARGUMENTS)
     {
          perror("Insufficient arguments passed to the program\n");
          exit(1);
     }

     // create shared memory object
     char *sharedMemoryObject = createSharedMemoryObject();

     // create child process 1
     pid_t childPid = fork();

     // unsuccessful creation
     if (childPid < 0)
     {
          perror("Error forking child process\n");
          exit(1);
     }

     // child process 1 - read from file and write to shared memory object
     if (childPid == 0)
     {
          // open file associated with command line argument for reading
          FILE *fp = fopen(argv[1], "r");
          if (fp == NULL)
          {
               perror("Error opening file\n");
               exit(1);
          }

          char buffer[BUFFER_SIZE];

          // will hold all commands from file
          char fileContent[BUFFER_SIZE] = "";

          // read into buffer from file
          while (fgets(buffer, BUFFER_SIZE, fp) != NULL)
          {
               // because buffer will be overwritten every iteration ... 
               // ..combine buffer contents with collective fileContent 
               strcat(fileContent, buffer);
          }

          // copy all commands to shared memory object
          strcpy(sharedMemoryObject, fileContent);

          fclose(fp);
          exit(0); // success
     }

     // parent process 1
     if (childPid > 0)
     {
          // wait until child process 1 is done copying from file to shared memory object
          wait(NULL);

          // dynamic array to hold all commands
          char *allCommands = (char *)malloc(BUFFER_SIZE);
          strcpy(allCommands, sharedMemoryObject);

          // tokenize single command from all commands
          char *command = strtok(allCommands, "\n");

          // read commands one by one, until there is no other command left
          while (command != NULL)
          {
               // create child process 2
               pid_t childPid2 = fork();

               // unsuccessful
               if (childPid2 < 0)
               {
                    perror("Error forking child process 2\n");
                    exit(1);
               }

               // child process 2 - create a pipe
               if (childPid2 == 0)
               {
                    int pipefd[2];
                    if (pipe(pipefd) == -1)
                    {
                         perror("Failed to create a pipe\n");
                         exit(1);
                    }

                    // create child process 3
                    pid_t childPid3 = fork();

                    // unsuccessful
                    if (childPid3 < 0)
                    {
                         perror("Error forking child process 3\n");
                         exit(1);
                    }

                    // child process 3 - execute POSIX commands and write them to pipe
                    if (childPid3 == 0)
                    {
                         // close reading-end of pipe (no need to read)
                         close(pipefd[0]);

                         // redirect anything written to stndout (console) to the write end of the pipe using dup2()
                         dup2(pipefd[1], STDOUT_FILENO);

                         // argument list to execute
                         // sh = use shell to run commands
                         // -c = specifies that next arg is the one to run
                         // commands = holds a single POSIX command
                         // NULL = convention
                         char *args[] = {"sh", "-c", command, NULL};
                         // swap out current child process (child process 3) for a new process that will execute the POSIX commands
                         execvp(args[0], args);

                         // at this point, control flow should have shifted to a new process, if it did not, there is an error
                         perror("Failed to create new process using execvp()\n");
                         exit(1);
                    }

                    // parent process 2 - read output of command from pipe
                    if (childPid3 > 0)
                    {
                         // close writing end of pipe (no need to write)
                         close(pipefd[1]);

                         // array to hold output of a SINGLE POSIX command
                         char output[BUFFER_SIZE];

                         // read from pipe into output
                         int bytesRead = read(pipefd[0], output, BUFFER_SIZE);

                         if (bytesRead < 0)
                         {
                              perror("Error reading from pipe from parent process 2");
                              exit(1);
                         }

                         // NULL terminate where output ends
                         output[bytesRead] = '\0';

                         // write output from buffer to file passed as argument
                         writeOutput(command, output);

                         // close reading end of pipe after reading from it
                         close(pipefd[0]);

                         exit(0);
                    }
               }

               // parent process 3 - get next POSIX command
               if (childPid2 > 0)
               {
                    // wait for child process 3 to finish executing commands...
                    // ..and wait for parent process 2 to output them to output.txt
                    wait(NULL);

                    // find and tokenize next command
                    command = strtok(NULL, "\n");
               }
          }

          // release shared memory object
          munmap(sharedMemoryObject, BUFFER_SIZE);
          shm_unlink(SHM_NAME);

          // release data allocated dynamically
          free(allCommands);
     }
     return 0;
}