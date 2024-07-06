/*
 -------------------------------------
 File: sudoku.c

 This program creates works with multi-threading to validate a 9x9 Sudoku
 puzzle and return whether the puzzle is valid by checking the following:

 1) Each row
 2) Each column
 3) Each 3x3 sub-grid

The program reads the Sudoku puzzle from a file, creates 11 threads to check
 -------------------------------------
 Author:  Vicky Sekhon, Yafet Tegbaru
 ID:      169024498, 210480140
 Email:   sekh4498@mylaurier.ca, tegb0140@mylaurier.ca
 Version  2024-07-02
 -------------------------------------
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// define constants

#define NUM_THREADS 11              // 9 threads to check each 3x3 sub-grid and 2 threads to each row and column respectively
#define PUZZLE sample_in_sudoku.txt // file containing the sudoku puzzle
#define SIZE 9                      // 9x9 sudoku puzzle

// function prototypes

void *validateRow(void *param);
void *validateColumn(void *param);
void *validateSubgrid(void *param);
void readSudokuPuzzleFromFile(const char *filename);
void printSudokuPuzzleToConsole();

// structure for passing data to threads
typedef struct
{ 
  int row;
  int column;
} parameters;

int sudoku[SIZE][SIZE]; // sudoku puzzle grid

int result[NUM_THREADS] = {0}; // result array to store validity of each thread's check

int main()
{
  readSudokuPuzzleFromFile(PUZZLE);
  printSudokuPuzzleToConsole();

  pthread_t threads[NUM_THREADS]; // array to store each thread
  parameters *data[NUM_THREADS];  // create data array to pass to threads

  // thread that checks rows
  data[0] = (parameters *)malloc(sizeof(parameters)); 
  data[0]->row = 0; // default to 0th row index
  data[0]->column = 0; // default to 0th column index

  // create row thread
  pthread_create(&threads[0], NULL, validateRow, data[0]);

  // thread that checks columns
  data[1] = (parameters *)malloc(sizeof(parameters));
  data[1]->row = 0; // default to 0th row index
  data[1]->column = 0; // default to 0th column index

  // create column thread
  pthread_create(&threads[1], NULL, validateColumn, data[1]);

  // create threads to check 3x3 sub-grids
  int thread_index = 2; // start at index 2 since 0 and 1 are used for row and column threads

  for (int i = 0; i < SIZE; i += 3) 
  { // loop through rows
    for (int j = 0; j < SIZE; j += 3)
    { // loop through columns
      data[thread_index] = (parameters *)malloc(sizeof(parameters));
      data[thread_index]->row = i; // set row index
      data[thread_index]->column = j; // set column index

      // create sub-grid thread
      pthread_create(&threads[thread_index], NULL, validateSubgrid, data[thread_index]);
      
      thread_index++; // move to next thread index
    }
  }

  // wait for all threads to complete
  for (int i = 0; i < NUM_THREADS; i++)
  {
    pthread_join(threads[i], NULL);
    free(data[i]);
  }

  // check the results
  for (int i = 0; i < NUM_THREADS; i++)
  {
    if (result[i] == 0) // if any thread returns 1, the puzzle is invalid 
    {
      printf("Sudoku puzzle is valid.\n");
      return 0;
    }
  }
  printf("Sudoku puzzle is invalid.\n");
  return 0;
}

void printSudokuPuzzleToConsole()
{
  printf("Sudoku Puzzle:\n");

  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      printf("%d ", sudoku[i][j]);
    }
    printf("\n"); // new line after each row
  }
}

void readSudokuPuzzleFromFile(const char *filename)
{
  FILE *file = fopen(filename, "r"); // open file for reading

  if (file == NULL)
  {
    perror("Unable to open file");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < SIZE; i++)
  { 
    for (int j = 0; j < SIZE; j++)
    {
      fscanf(file, "%d", &sudoku[i][j]); // read integers from file into the sudoku array
    }
  }

  fclose(file); // close file after reading
}

void *validateRow(void *param)
{
  for (int i = 0; i < SIZE; i++)
  {
    int check[SIZE + 1] = {0};
    for (int j = 0; j < SIZE; j++)
    {
      int num = sudoku[i][j];
      if (num < 1 || num > 9 || check[num] == 1)
      {
        result[0] = 0;
        pthread_exit(NULL);
      }
      else
      {
        check[num] = 1;
      }
    }
  }
  result[0] = 1;
  pthread_exit(NULL);
}

void *validateColumn(void *param)
{
  for (int j = 0; j < SIZE; j++)
  {
    int check[SIZE + 1] = {0}; // initialize check array to 0 
    for (int i = 0; i < SIZE; i++)
    {
      int num = sudoku[i][j]; // get number from sudoku puzzle
      if (num < 1 || num > 9 || check[num] == 1)  // check if number is valid
      {
        result[1] = 0; // set result to 0 if invalid
        pthread_exit(NULL);
      }
      else
      {
        check[num] = 1; // set check array to 1 if valid
      }
    }
  }
  
  result[1] = 1;
  pthread_exit(NULL);
}

void *validateSubgrid(void *param)
{
  parameters *p = (parameters *)param;

  int check[SIZE + 1] = {0}; // initialize check array to 0
  for (int i = p->row; i < p->row + 3; i++) // stop condition = 3rd row
  {
    for (int j = p->column; j < p->column + 3; j++) // stop condition = 3rd column
    {
      int num = sudoku[i][j]; // get number from 3x3 sub-grid

      if (num < 1 || num > 9 || check[num] == 1) // check if number is valid
      {
        int index = (p->row / 3) * 3 + (p->column / 3) + 2; // index for result array
        result[index] = 0; // set result to 0 if invalid
        pthread_exit(NULL);
      }
      else
      {
        check[num] = 1; // set check array to 1 if valid
      }
    }
  }

  int index = (p->row / 3) * 3 + (p->column / 3) + 2; // index for result array
  result[index] = 1; // set result to 1 if valid 
  pthread_exit(NULL);
}