


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 11
#define SIZE 9

// Structure for passing data to threads
typedef struct {
    int row;
    int column;
} parameters;

// Sudoku puzzle grid
int sudoku[SIZE][SIZE];

// Result array to store validity of each thread's check
int result[NUM_THREADS] = {0};

void* check_row(void* param);
void* check_column(void* param);
void* check_subgrid(void* param);

// Function to read the Sudoku puzzle from the file
void read_sudoku(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fscanf(file, "%d", &sudoku[i][j]);
        }
    }

    fclose(file);
}

// Function to print the Sudoku puzzle
void print_sudoku() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", sudoku[i][j]);
        }
        printf("\n");
    }
}

int main() {
    read_sudoku("sample_in_sudoku.txt");

    printf("Sudoku Puzzle:\n");
    print_sudoku();

    pthread_t threads[NUM_THREADS];
    parameters* data[NUM_THREADS];

    // Create thread to check rows
    data[0] = (parameters*) malloc(sizeof(parameters));
    data[0]->row = 0;
    data[0]->column = 0;
    pthread_create(&threads[0], NULL, check_row, data[0]);

    // Create thread to check columns
    data[1] = (parameters*) malloc(sizeof(parameters));
    data[1]->row = 0;
    data[1]->column = 0;
    pthread_create(&threads[1], NULL, check_column, data[1]);

    // Create threads to check 3x3 sub-grids
    int thread_index = 2;
    for (int i = 0; i < SIZE; i += 3) {
        for (int j = 0; j < SIZE; j += 3) {
            data[thread_index] = (parameters*) malloc(sizeof(parameters));
            data[thread_index]->row = i;
            data[thread_index]->column = j;
            pthread_create(&threads[thread_index], NULL, check_subgrid, data[thread_index]);
            thread_index++;
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        free(data[i]);
    }

    // Check the results
    for (int i = 0; i < NUM_THREADS; i++) {
        if (result[i] == 0) {
            printf("Sudoku puzzle is invalid.\n");
            return 0;
        }
    }

    printf("Sudoku puzzle is valid.\n");
    return 0;
}

void* check_row(void* param) {
    parameters* p = (parameters*) param;
    for (int i = p->row; i < SIZE; i++) {
        int check[SIZE + 1] = {0};
        for (int j = p->column; j < SIZE; j++) {
            int num = sudoku[i][j];
            if (num < 1 || num > 9 || check[num] == 1) {
                result[0] = 0;
                pthread_exit(NULL);
            } else {
                check[num] = 1;
            }
        }
    }
    result[0] = 1;
    pthread_exit(NULL);
}

void* check_column(void* param) {
    parameters* p = (parameters*) param;
    for (int j = p->column; j < SIZE; j++) {
        int check[SIZE + 1] = {0};
        for (int i = p->row; i < SIZE; i++) {
            int num = sudoku[i][j];
            if (num < 1 || num > 9 || check[num] == 1) {
                result[1] = 0;
                pthread_exit(NULL);
            } else {
                check[num] = 1;
            }
        }
    }
    result[1] = 1;
    pthread_exit(NULL);
}

void* check_subgrid(void* param) {
    parameters* p = (parameters*) param;
    int check[SIZE + 1] = {0};
    for (int i = p->row; i < p->row + 3; i++) {
        for (int j = p->column; j < p->column + 3; j++) {
            int num = sudoku[i][j];
            if (num < 1 || num > 9 || check[num] == 1) {
                int index = (p->row / 3) * 3 + (p->column / 3) + 2;
                result[index] = 0;
                pthread_exit(NULL);
            } else {
                check[num] = 1;
            }
        }
    }
    int index = (p->row / 3) * 3 + (p->column / 3) + 2;
    result[index] = 1;
    pthread_exit(NULL);
}
