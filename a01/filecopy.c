/*
-------------------------------------
File:    filecopy.c
Project: assignment 1
Enables various directory operations, enhancing understanding of file system manipulations and system calls.
-------------------------------------
Authors:  Yafet Tegbaru, Vicky Sekhon
IDs:      210480140, 169024498
Emails:   tegb0140@mylaurier.ca, sekh4498@mylaurier.ca
Version   2024-05-20
-------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#define COMMAND_LINE_PARAMETERS 3

int main(int argc, char *argv[])
{

    FILE *inputFile, *outputFile;

    char buffer[1024];

    // Bytes read during single iteration of copy process
    size_t bytesRead;
    // Bytes written during single iteration of copy process
    size_t bytesWritten;

    // Check if the correct number of arguments are passed
    if (argc != COMMAND_LINE_PARAMETERS)
    {
        printf("Insufficient parameters passed.\n");
        return 1; // Abnormal termination
    }

    // Open the input file for reading
    inputFile = fopen(argv[1], "rb");
    if (inputFile == NULL)
    {
        perror("Error opening input file"); // Prints why the open failed
        return 2;                           // Abnormal termination
    }

    // Create and open the output file for writing (overwrite if exists)
    outputFile = fopen(argv[2], "wb");
    if (outputFile == NULL)
    {
        fclose(inputFile);                   // Close input file before exiting
        perror("Error opening output file"); // Prints why the open failed
        return 3;                            // Abnormal termination
    }

    // Copy the content from input to output file
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), inputFile)) > 0)
    {
        bytesWritten = fwrite(buffer, 1, bytesRead, outputFile);

        // Not all bytes read were written to the output file (data is missing)
        if (bytesWritten != bytesRead)
        {
            perror("Error writing to output file"); // Print write error
            fclose(inputFile);
            fclose(outputFile);
            return 4; // Abnormal termination
        }
    }

    // Check for any error associated with the input file
    if (ferror(inputFile))
    {
        perror("Error reading from input file"); // Print read error
        fclose(inputFile);
        fclose(outputFile);
        return 5; // Abnormal termination
    }

    // Copy is complete, close both files
    fclose(inputFile);
    fclose(outputFile);

    printf("The contents of file '%s' have been successfully copied into '%s' file.\n", argv[1], argv[2]);

    return 0; // Normal termination
}