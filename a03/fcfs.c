/*
 -------------------------------------
 File: fcfs.c

 This program reads the Thread ID, Arrival Time, and Burst Time from a file and
 "simulates" scheduling these algorithms based on the First Come First Serve (FCFS)
 CPU scheduling algorithm. The threads in the file "sample_in_schedule.txt" are
 non-preemptive to allow for the simulation of the FCFS algorithm.

 The then program calculates the completion time, turn-around time, and waiting time
 of the threads using the following formulas:

  • Completion time = (Burst Time n) + (Burst Time n-1) + ... + (Burst Time 1)
  • Turn-around time = Completion time - Arrival time
  • Waiting time = Turn-around time - Burst Time

 The average waiting time and turn-around times are then calculated.
 -------------------------------------
 Author:  Vicky Sekhon, Yafet Tegbaru
 ID:      169024498, 210480140
 Email:   sekh4498@mylaurier.ca, tegb0140@mylaurier.ca
 Version  2024-07-05
 -------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>

// information associated with each thread
typedef struct
{
  int p_id;
  int arrival_time;
  int burst_time;
  int completion_time;
  int waiting_time;
  int turn_around_time;
} threadInformation;

// function prototypes

void readSchedulingInformationFromFile(const char *filename, threadInformation *threads, int *count);
float calculateCompletionTime(int currentTime, float burstTime);
float calculateTurnAroundTime(float completionTime, float arrivalTime);
float calculateWaitingTime(float turnAroundTime, float burstTime);
void performIndividualCalculations(threadInformation *threads, int count);
float calculateAverageWaitingTime(float totalWaitingTime, int count);
float calculateAverageTurnAroundTime(float totalTurnaroundTime, int count);
void performAveragesCalculations(threadInformation *threads, int count, float *avg_waiting_time, float *avg_turn_around_time);
void printThreadTable(threadInformation *threads, int count);

int main()
{
  threadInformation threads[100]; // create array to hold thread information (100 threads max)

  int count; // track number of threads in the file

  float avg_waiting_time, avg_turn_around_time; // initialize average waiting and turn-around times

  readSchedulingInformationFromFile("sample_in_schedule.txt", threads, &count); // transfer thread information from file into array

  performIndividualCalculations(threads, count); // calculate completion time, turn-around time, and waiting time for each thread

  performAveragesCalculations(threads, count, &avg_waiting_time, &avg_turn_around_time); // calculate average waiting and turn-around times

  printThreadTable(threads, count); // print the thread table

  // display averages
  printf("The average waiting time: %.2f\n", avg_waiting_time);
  printf("The average turn-around time: %.2f\n", avg_turn_around_time);

  return 0;
}

void readSchedulingInformationFromFile(const char *filename, threadInformation *threads, int *count)
{
  FILE *file = fopen(filename, "r"); // open file for reading

  if (file == NULL)
  {
    perror("Unable to open file");
    exit(EXIT_FAILURE);
  }

  *count = 0; // tracks the total # of threads in the file

  // read the thread ID, thread arrival time, and thread burst time from the file until end of file is reached
  while (fscanf(file, "%d, %d, %d", &threads[*count].p_id, &threads[*count].arrival_time, &threads[*count].burst_time) != EOF)
  {
    (*count)++;
  }

  fclose(file); // close file after reading
}

float calculateCompletionTime(int currentTime, float burstTime)
{
  return currentTime + burstTime;
}

float calculateTurnAroundTime(float completionTime, float arrivalTime)
{
  return completionTime - arrivalTime;
}

float calculateWaitingTime(float turnAroundTime, float burstTime)
{
  return turnAroundTime - burstTime;
}

void performIndividualCalculations(threadInformation *threads, int count)
{
  int current_time = 0;

  for (int i = 0; i < count; i++)
  { // loop through each thread
    threads[i].completion_time = calculateCompletionTime(current_time, threads[i].burst_time);
    threads[i].turn_around_time = calculateTurnAroundTime(threads[i].completion_time, threads[i].arrival_time);
    threads[i].waiting_time = calculateWaitingTime(threads[i].turn_around_time, threads[i].burst_time);

    current_time = threads[i].completion_time; // update current time to the end of the current thread
  }
}

float calculateAverageWaitingTime(float totalWaitingTime, int count)
{
  return totalWaitingTime / count;
}

float calculateAverageTurnAroundTime(float totalTurnaroundTime, int count)
{
  return totalTurnaroundTime / count;
}

void performAveragesCalculations(threadInformation *threads, int count, float *avg_waiting_time, float *avg_turn_around_time)
{
  int totalWaitingTime = 0;
  int totalTurnaroundTime = 0;

  // calculate the total waiting time and turn-around times
  for (int i = 0; i < count; i++)
  {
    totalWaitingTime += threads[i].waiting_time;
    totalTurnaroundTime += threads[i].turn_around_time;
  }

  *avg_waiting_time = calculateAverageWaitingTime((float)totalWaitingTime, count);
  *avg_turn_around_time = calculateAverageTurnAroundTime((float)totalTurnaroundTime, count);
}

void printThreadTable(threadInformation *threads, int count)
{

  // header for the thread table
  printf("Thread ID\tArrival Time\tBurst Time\tCompletion Time\tTurn-Around Time\t\tWaiting Time\n");

  for (int i = 0; i < count; i++)
  { // loop through each thread
    printf("%d\t\t\t\t%d\t\t\t\t%d\t\t\t\t%d\t\t\t\t%d\t\t\t\t%d\n",
           threads[i].p_id,
           threads[i].arrival_time, threads[i].burst_time,
           threads[i].completion_time, threads[i].turn_around_time,
           threads[i].waiting_time);
  }
}