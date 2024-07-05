/*
 -------------------------------------
 File: fcfs.c
 Project: sekh4498_a01, tegb0140_a01
 -------------------------------------
 Author:  Vicky Sekhon, Yafet Tegbaru
 ID:      169024498, 210480140
 Email:   sekh4498@mylaurier.ca, tegb0140@mylaurier.ca
 Version  2024-07-05
 -------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>

// Structure to hold thread information
typedef struct {
  int p_id;
  int arr_time;
  int burst_time;
  int completion_time;
  int waiting_time;
  int turn_around_time;
} threadinfo;

void read_schedule(const char *filename, threadinfo *threads, int *count) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    perror("Unable to open file");
    exit(EXIT_FAILURE);
  }

  *count = 0;
  while (fscanf(file, "%d, %d, %d", &threads[*count].p_id,
                &threads[*count].arr_time,
                &threads[*count].burst_time) != EOF) {
    (*count)++;
  }

  fclose(file);
}

void calculate_times(threadinfo *threads, int count) {
  int current_time = 0;

  for (int i = 0; i < count; i++) {
    if (current_time < threads[i].arr_time) {
      current_time = threads[i].arr_time;
    }
    threads[i].completion_time = current_time + threads[i].burst_time;
    threads[i].turn_around_time =
        threads[i].completion_time - threads[i].arr_time;
    threads[i].waiting_time =
        threads[i].turn_around_time - threads[i].burst_time;
    current_time = threads[i].completion_time;
  }
}

void calculate_averages(threadinfo *threads, int count, float *avg_waiting_time,
                        float *avg_turn_around_time) {
  int total_waiting_time = 0;
  int total_turn_around_time = 0;

  for (int i = 0; i < count; i++) {
    total_waiting_time += threads[i].waiting_time;
    total_turn_around_time += threads[i].turn_around_time;
  }

  *avg_waiting_time = (float)total_waiting_time / count;
  *avg_turn_around_time = (float)total_turn_around_time / count;
}

void print_thread_info(threadinfo *threads, int count) {
  printf("Thread ID\tArrival Time\tBurst Time\tCompletion Time\tTurn-Around "
         "Time\tWaiting Time\n");
  for (int i = 0; i < count; i++) {
    printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t\t%d\n", threads[i].p_id,
           threads[i].arr_time, threads[i].burst_time,
           threads[i].completion_time, threads[i].turn_around_time,
           threads[i].waiting_time);
  }
}

int main() {
  threadinfo threads[100];
  int count;
  float avg_waiting_time, avg_turn_around_time;

  read_schedule("sample_in_schedule.txt", threads, &count);
  calculate_times(threads, count);
  calculate_averages(threads, count, &avg_waiting_time, &avg_turn_around_time);

  print_thread_info(threads, count);
  printf("The average waiting time: %.2f\n", avg_waiting_time);
  printf("The average turn-around time: %.2f\n", avg_turn_around_time);

  return 0;
}
