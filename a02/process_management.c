/*
 -------------------------------------
 File: process_managemente.c
 Project: sekh4498_a02, tegb0140_a02

 This file contains a program to read 
 and execute shell commands from a file 
 using multiple processes.
 -------------------------------------
 Authors:  Vicky Sekhon, Yafet Tegbaru
 IDs:      169024498, 210480140
 Emails:   sekh4498@mylaurier.ca, tegb0140@mylaurier.ca
 Version   2024-06-04
 -------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>

void writeOutput(const char *output) {
    FILE *fp = fopen("output.txt", "a");
    if (fp) {
        fprintf(fp, "%s", output);
        fclose(fp);
    } else {
        perror("Failed to open output file");
        exit(1);
    }
}

void readCommandsFromFile(const char *filename, char *shm_ptr) {
    FILE *file = fopen(filename, "r");
    if (file) {
        while (fgets(shm_ptr + strlen(shm_ptr), 256, file));
        fclose(file);
    } else {
        perror("Failed to open input file");
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        exit(1);
    }

    size_t shm_size = 8192;
    int shm_fd = shm_open("/shm_commands", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, shm_size);
    char *shm_ptr = mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if (fork() == 0) {
        readCommandsFromFile(argv[1], shm_ptr);
        exit(0);
    }
    wait(NULL);

    char *command = strtok(shm_ptr, "\n");
    while (command) {
        int pipe_fd[2];
        pipe(pipe_fd);

        if (fork() == 0) {
            dup2(pipe_fd[1], STDOUT_FILENO);
            dup2(pipe_fd[1], STDERR_FILENO);
            close(pipe_fd[0]);
            close(pipe_fd[1]);
            execlp("/bin/sh", "sh", "-c", command, (char *)NULL);
            perror("exec failed");
            exit(1);
        } else {
            close(pipe_fd[1]);
            char buffer[4096];
            ssize_t nbytes;
            while ((nbytes = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) {
                buffer[nbytes] = '\0';
                writeOutput(buffer);
            }
            close(pipe_fd[0]);
            wait(NULL);
        }
        command = strtok(NULL, "\n");
    }

    munmap(shm_ptr, shm_size);
    shm_unlink("/shm_commands");
    return 0;
}
