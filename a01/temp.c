/*
 -------------------------------------
 File: temp.c
 Project: sekh4498_a01 
 This file contains the library functions that we are playing around with to see functionality.
 -------------------------------------
 Author:  Vicky Sekhon, Yafet Tegbaru
 ID:      169024498, 210480140
 Email:   sekh4498@mylaurier.ca, tegb0140@mylaurier.ca
 Version  2024-05-16
 -------------------------------------
 */

// Include the necessary libraries 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

int main() {
     printf("Hello, World!\n");

     // opens the directory
     DIR *hello = opendir("./this");
     if (hello == NULL) {
          printf("Directory not found\n");
     } else {
          printf("Directory found\n");
     }

     // reads the directory
     struct dirent *entry;
     while ((entry = readdir(hello)) != NULL) {
          printf("%s\n", entry->d_name);
     }

     // printf("%ld", telldir(hello)); gives the current position of a file in the directory stream

     // rewinddir(hello); resets the position of the directory stream to the beginning
     
     // readdir_r is a reentrant version of readdir



     // closes the directory
     int check = closedir(hello);
     if (check == 0) {
          printf("Directory closed\n");
     } else {
          printf("Directory not closed\n");
     }

}