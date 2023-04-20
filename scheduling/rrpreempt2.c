#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct task {
    int id;
    int arrival_time;
    int worst_case_exec_time;
    int io_time;
    int io_interval;
    int remaining_exec_time;
    int remaining_io_time;
    int io_countdown;
    struct task *next;
} Task;

Task *task_queue = NULL;

// Function to add a new task to the task queue
void add_task(Task **queue, Task *new_task) {
    if (*queue == NULL || (*queue)->arrival_time > new_task->arrival_time) {
        new_task->next = *queue;
        *queue = new_task;
    }
    else {
        Task *current_task = *queue;
        while (current_task->next != NULL && current_task->next->arrival_time <= new_task->arrival_time) {
            current_task = current_task->next;
        }
        new_task->next = current_task->next;
        current_task->next = new_task;
    }
}

// Function to get the next task from the task queue
Task *get_task() {
    if (task_queue == NULL) {
        return NULL;
    }
    Task *next_task = task_queue;
    task_queue = task_queue->next;
    next_task->next = NULL;
    return next_task;
}

// Function to execute a task for a given quantum of time
void execute_task(Task *task, int quantum) {
    int exec_time = task->remaining_exec_time;
    if (exec_time > quantum) {
        exec_time = quantum;
    }
    task->remaining_exec_time -= exec_time;
    if (task->remaining_exec_time == 0) {
        printf("Task %d completed.\n", task->id);
        free(task);
        return;
    }
    if (task->remaining_io_time > 0) {
        task->remaining_io_time -= exec_time;
        if (task->remaining_io_time == 0) {
            task->io_countdown = task->io_interval;
        }
    }
    else {
        task->io_countdown -= exec_time;
        if (task->io_countdown <= 0) {
            task->remaining_io_time = task->io_time;
        }
    }
    add_task(&task_queue, task);
}

int main() {
    srand(time(NULL));
    
    // Open the tasks file
    FILE *file = fopen("tasks.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }
    
    // Read the tasks from the file and add them to the task queue
    int id, arrival_time, worst_case_exec_time, io_time, io_interval;
    while (fscanf(file, "%d %d %d %d %d", &id, &arrival_time, &worst_case_exec_time, &io_time, &io_interval) != EOF) {
        Task *new_task = (Task *) malloc(sizeof(Task));
        new_task->id = id;
        new_task->arrival_time = arrival_time;
        new_task->worst_case_exec_time = worst_case_exec_time;
        new_task->io_time = io_time;
        new_task->io_interval = io_interval;
        new_task->remaining_exec_time = (int) (worst_case_exec_time * (0.6 + (0.4 * ((double) rand() / (double) RAND_MAX))));
        new_task->remaining_io_time = 0;
        new_task->io_countdown = io_interval;
        add_task(&task_queue, new_task);
        printf("Added task number %d with execution time %d\n", id, new_task->remaining_exec_time);
    }

    // Simulate the execution of the tasks in the task queue
    int quantum = 5;
    int current_time = 0;
    Task *current_task = NULL;
    while (task_queue != NULL) {
        current_task = get_task();
        if (current_task->arrival_time > current_time) {
            current_time = current_task->arrival_time;
        }
        while (current_task->remaining_exec_time > 0) {
            execute_task(current_task, quantum);
            current_time += quantum;
            if (task_queue != NULL && task_queue->arrival_time <= current_time) {
                Task *new_task = get_task();
                if (new_task->remaining_exec_time > current_task->remaining_exec_time) {
                    add_task(&task_queue, new_task);
                    break;
                }
                else {
                    execute_task(current_task, new_task->arrival_time - current_time);
                    current_time = new_task->arrival_time;
                    add_task(&task_queue, current_task);
                    current_task = new_task;
                }
            }
        }
    }

    // Close the tasks file
    fclose(file);

    return 0;
}
