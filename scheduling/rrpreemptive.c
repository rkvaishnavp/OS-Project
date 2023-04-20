#include <stdio.h>
#include <stdlib.h>

#define MAX_TASKS 10
#define QUANTUM 2

// Struct to store information about each task
typedef struct task {
    int task_no;
    int arrival_time;
    int cpu_time[MAX_TASKS];
    int io_time[MAX_TASKS];
    int num_bursts;
    int curr_burst;
    int time_left;
    int io_done;
    int waiting_time;
} Task;

// Global variables
Task tasks[MAX_TASKS];
int num_tasks;
int curr_time = 0;

// Function to read task information from input file
void read_tasks(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Unable to open input file!\n");
        exit(1);
    }

    num_tasks = 0;
    while (fscanf(file, "%d", &tasks[num_tasks].task_no) != EOF) {
        fscanf(file, "%d", &tasks[num_tasks].arrival_time);
        tasks[num_tasks].num_bursts = 0;

        int i = 0;
        while (fscanf(file, "%d", &tasks[num_tasks].cpu_time[i]) != EOF) {
            fscanf(file, "%d", &tasks[num_tasks].io_time[i]);
            tasks[num_tasks].num_bursts++;
            i++;
        }

        tasks[num_tasks].curr_burst = 0;
        tasks[num_tasks].time_left = tasks[num_tasks].cpu_time[0];
        tasks[num_tasks].io_done = 1;
        tasks[num_tasks].waiting_time = 0;

        num_tasks++;
    }

    fclose(file);
}

// Function to simulate Round Robin scheduling
void round_robin() {
    int finished_tasks = 0;
    int curr_task = -1;
    int quantum_left = 0;

    while (finished_tasks < num_tasks) {
        // Find the task with the earliest arrival time that is ready to run
        int next_task = -1;
        for (int i = 0; i < num_tasks; i++) {
            if (tasks[i].arrival_time <= curr_time && tasks[i].time_left > 0 && !tasks[i].io_done) {
                if (next_task == -1 || tasks[i].arrival_time < tasks[next_task].arrival_time) {
                    next_task = i;
                }
            }
        }

        if (next_task != -1) {
            // If a task was found, switch to it if necessary
            if (curr_task == -1 || curr_task != next_task) {
                curr_task = next_task;
                quantum_left = QUANTUM;
            }

            // Run the task for a time slice
            tasks[curr_task].time_left--;
            quantum_left--;

            // If the task finished its burst, switch to IO mode
            if (tasks[curr_task].time_left == 0) {
                tasks[curr_task].io_done = 0;
                tasks[curr_task].curr_burst++;
                if (tasks[curr_task].curr_burst == tasks[curr_task].num_bursts) {
                    // If the task has finished all its bursts, mark it as finished
                    finished_tasks++;
                } else {
                    tasks[curr_task].time_left = tasks[curr_task].io_time[tasks[curr_task].curr_burst];
                }
            }

            // If the quantum has been used up, switch to the next task
            if (quantum_left == 0) {
                curr_task = -1;
            }
        } else {
           // If no task is ready to run, increment the current time and add waiting time for all tasks in the queue
curr_time++;
for (int i = 0; i < num_tasks; i++) {
if (tasks[i].arrival_time <= curr_time && tasks[i].time_left > 0 && !tasks[i].io_done) {
tasks[i].waiting_time++;
}
}
}
}
}

// Function to print the results of the simulation
void print_results() {
printf("Task\tWaiting Time\n");
for (int i = 0; i < num_tasks; i++) {
printf("%d\t%d\n", tasks[i].task_no, tasks[i].waiting_time);
}
}

int main() {
read_tasks("tasks.txt");
round_robin();
print_results();
return 0;
}
