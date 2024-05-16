/*
 -------------------------------------
 File: directory.c
 Project: sekh4498_a01 
 This file contains the functions that are used to create, delete, and list directories.
 -------------------------------------
 Author:  Vicky Sekhon, Yafet Tegbaru
 ID:      169024498, 210480140
 Email:   sekh4498@mylaurier.ca, tegb0140@mylaurier.ca
 Version  2024-05-16
 -------------------------------------
 */


#include <stdio.h>  // accesses standard input/output functions 
#include <stdlib.h> // accesses memory allocation and exit functions
#include <string.h> // accesses string manipulation functions
#include <sys/stat.h> // accesses directory-related operations
#include <dirent.h> // accesses directory-related operations
#include <unistd.h> // accesses the chdir() function

// function prototypes

// type of function that user wants to perform
int userWantsToCreateADirectory(const char userInput[]);
int userWantsToRemoveADirectory(const char userInput[]);
int userWantsToGetCurrentWorkingDirectory(const char userInput[]);
int userWantsToChangeDirectoryOneLevelUp(const char userInput[]);
int userWantsToReadTheContentsOfTheDirectory(const char userInput[]);
int userWantsToCloseTheCurrentDirectory(const char userInput[]);

// functions that perform the user's desired action
void createDirectory();
void removeDirectory();
void getCurrentWorkingDirectory();
void changeDirectoryOneLevelUp();
void readTheContentsOfTheDirectory();
void closeCurrentWorkingDirectory(); 

int main() {

     char userInput[10]; 

     // run indefinitely until the user enters 'q'
     while (1) {

          printf("1. Create a directory\n");
          printf("2. Remove a directory\n");
          printf("3. Print current working directory\n");
          printf("4. Change directory one level up\n");
          printf("5. Read the contents of the directory\n");
          printf("6. Close the current directory\n");
          printf("q. Exit the program\n");

          scanf("%s", userInput);

          if (userWantsToCreateADirectory(userInput) == 0) {
               createDirectory();
          } else if (userWantsToRemoveADirectory(userInput) == 0) {
               removeDirectory();
          } else if (userWantsToGetCurrentWorkingDirectory(userInput) == 0) {
               getCurrentWorkingDirectory();
          } else if (userWantsToChangeDirectoryOneLevelUp(userInput) == 0) {
               changeDirectoryOneLevelUp();
          } else if (userWantsToReadTheContentsOfTheDirectory(userInput) == 0) {
               readTheContentsOfTheDirectory();
          } else if (userWantsToCloseTheCurrentDirectory(userInput) == 0) {
               closeCurrentWorkingDirectory();
          } else { 
               return printf(" `q` was entered. Exiting program\n");
          }
     }
}

int userWantsToCreateADirectory(const char userInput[]) { return strcmp(userInput, "1"); }

int userWantsToRemoveADirectory(const char userInput[]) { return strcmp(userInput, "2"); }

int userWantsToGetCurrentWorkingDirectory(const char userInput[]) { return strcmp(userInput, "3"); }

int userWantsToChangeDirectoryOneLevelUp(const char userInput[]) { return strcmp(userInput, "4"); }

int userWantsToReadTheContentsOfTheDirectory(const char userInput[]) { return strcmp(userInput, "5"); }

int userWantsToCloseTheCurrentDirectory(const char userInput[]) { return strcmp(userInput, "6"); }

// create a directory
void createDirectory() {
     char directoryName[100];
     printf("Directory name: ");
     scanf("%s", directoryName); 

     if (mkdir(directoryName) == 0) {
          printf("Directory named {%s} was created\n", directoryName);
     } else {
          printf("Directory named {%s} could not be created\n", directoryName);
     }
}

// remove a directory 
void removeDirectory() {
     char directoryName[100];
     printf("Directory name: ");
     scanf("%s", directoryName); 

     if (rmdir(directoryName) == 0) {
          printf("Directory named {%s} was removed\n", directoryName);
     } else {
          printf("Directory named {%s} could not be removed\n", directoryName);
     }
}

// get the current working directory
void getCurrentWorkingDirectory() {
     char cwd[1024];
     if (getcwd(cwd, sizeof(cwd)) != NULL) {
          printf("Current working directory is {%s}\n", cwd);
     } else {
          perror("getcwd() error");
     }
}

// change directory one level up
void changeDirectoryOneLevelUp() {
     chdir("..");
     getCurrentWorkingDirectory();
}

// read contents of the directory
void readTheContentsOfTheDirectory() {

     // open the current directory 
     DIR *workingDirectory = opendir(".");

     if (workingDirectory == NULL) {
          printf("Directory not found\n");
     } else {
          printf("Directory found\n");
     }

     // read all contents inside the directory, including file and folder names
     struct dirent *entry; 
     while ((entry = readdir(workingDirectory)) != NULL) {
          printf("%s\n", entry->d_name);
     }

     // close the directory after reading its contents
     int check = closedir(workingDirectory);

     if (check == 0) {
          printf("Directory closed\n");
     } else {
          printf("Directory not closed\n");
     }
}

// close the current working directory
void closeCurrentWorkingDirectory() {
     
     // get the current working directory
     DIR *cwd = opendir(".");

     // close it and get status
     int status = closedir(cwd);

     if (status == 0) {
          printf("Directory was closed successfully");
     } else {
          printf("Directory could not be closed");
     }
}