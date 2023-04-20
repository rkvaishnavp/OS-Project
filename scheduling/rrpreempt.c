#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Structure for a task
struct task {
    int id;
    int worst_case_execution_time;
    int io_time;
    int priority;
    int remaining_execution_time;
};

// Structure for a node in the task queue
struct node {
    struct task *task;
    struct node *next;
};

// Function to add a task to the task queue based on its priority
void add_task(struct node **queue, struct task *new_task) {
    struct node *new_node = (struct node*)malloc(sizeof(struct node));
    new_node->task = new_task;
    new_node->next = NULL;
    
    if (*queue == NULL) {
        *queue = new_node;
    }
    else if ((*queue)->task->priority > new_task->priority) {
        new_node->next = *queue;
        *queue = new_node;
    }
    else {
        struct node *current_node = *queue;
        while (current_node->next != NULL && current_node->next->task->priority <= new_task->priority) {
            current_node = current_node->next;
        }
        new_node->next = current_node->next;
        current_node->next = new_node;
    }
}

// Function to remove and return the first task in the task queue
struct task* get_task(struct node **queue) {
    if (*queue == NULL) {
        return NULL;
    }
    else {
        struct task *task = (*queue)->task;
        struct node *temp = *queue;
        *queue = (*queue)->next;
        free(temp);
        return task;
    }
}

// Function to simulate the execution of a task
void execute_task(struct task *task, int quantum) {
    printf("Executing task %d for quantum %d\n", task->id, quantum);
    
    if (task->remaining_execution_time > quantum) {
        task->remaining_execution_time -= quantum;
        task->priority--;
    }
    else {
        task->remaining_execution_time = 0;
    }
}

// Function to generate a random number between 60% and 100% of the worst case execution time
int get_actual_execution_time(int worst_case_execution_time) {
    return (rand() % 41 + 60) * worst_case_execution_time / 100;
}

// Main function
int main() {
    // Seed the random number generator with the current time
    srand(time(NULL));
    
    // Open the tasks file for reading
    FILE *fp = fopen("tasks.txt", "r");
    if (fp == NULL) {
        printf("Error: Could not open tasks file.\n");
        return 1;
    }
    
    // Read the tasks from the file and add them to the task queue
    struct node *task_queue = NULL;
    struct task *task;
    int id, w, i;
    while (fscanf(fp, "%d %d %d", &id, &w, &i) != EOF) {
        task = (struct task*)malloc(sizeof(struct task));
        task->id = id;
        task->worst_case_execution_time = w;
        task->io_time = i;
        task->priority = 5;
        task->remaining_execution_time = get_actual_execution_time(w);
        add_task(&task_queue, task);
    }
    fclose(fp);
    
    // Simulate the execution of the tasks in the task queue
    int quantum = 5;


    while (task_queue != NULL) {
        // Get the highest priority task from the task queue
        task = get_task(&task_queue);
        if (task == NULL) {
            break;
        }
        
        // Execute the task for the current quantum
        execute_task(task, quantum);
        
        // If the task still has remaining execution time, add it back to the task queue
        if (task->remaining_execution_time > 0) {
            add_task(&task_queue, task);
        }
        else {
            printf("Task %d completed.\n", task->id);
            free(task);
        }
    }

return 0;
}


// The code reads the tasks from the file "tasks.txt" and adds them to a priority queue based on
// their priority. The tasks are executed in a round-robin fashion using a quantum of 5 units of time.
// The actual execution time of each task is a random number between 60% and 100% of its worst case execution time.

// The priority of a task is initialized to 5, and is decremented by 1 every time the task is executed for a quantum.
// This means that tasks with higher priority will be executed first, but tasks that take longer to complete will have
// their priority reduced over time and eventually be executed less frequently.

// The code outputs a message for each task that completes, and frees the memory allocated for the task.


// 1 20 5
// 2 30 8
// 3 15 3
// 4 25 6
// 5 10 2
// 6 35 9
// 7 18 4
// 8 28 7
// 9 12 2
// 10 22 5