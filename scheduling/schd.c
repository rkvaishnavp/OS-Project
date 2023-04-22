#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESS 10
#define QUANTUM_TIME 4

// Struct to store information about each task
typedef struct process {
    int pid;
    int arrival_time;
    
    int cpu_burst[10];
    int io_burst[10];

    int curr_cpu;
    int curr_io;
    
    int cpu_time_left;
    struct process *next;
} Process ;

// Global variables
Process *queue = NULL;
int global_time = 0;
int num_process;
int count = 0;

// //for reading
void read_tasks(const char* filename) {
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    
    // Read the tasks from the file and add them to the task queue
   
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
}

void round_robin(){
 
    if(queue->arrival_time > global_time){
        global_time = queue->arrival_time;
    }

    if(queue->cpu_burst <= QUANTUM_TIME){
        global_time += queue->cpu_burst[queue->curr_cpu];
        queue->arrival_time = global_time + queue->io_burst[queue -> curr_io];

        queue->curr_cpu ++;
        queue->curr_io ++;
       

        Process *tmp = queue;
        tmp->next = NULL;

        queue = queue->next; 

        if(queue->cpu_burst[queue->curr_cpu] != 0) {  
            printf("Process with pid  %d ends its %d th burst at t= %d\n",queue->pid,queue->curr_cpu, global_time);    
            add_task(queue->next, tmp);
           }

        else{
            count++;
            printf("Process with pid %d ends at t= %d\n",queue->pid,global_time); 

            if (count == num_process){
                printf("End of schedule at time = %d\n", global_time);
            }
        }
    }
    else{
        global_time += QUANTUM_TIME;
        queue->arrival_time = global_time;

        queue->cpu_burst[queue->curr_cpu] -= QUANTUM_TIME;

        Process *tmp = queue;
        tmp->next = NULL;

        queue = queue->next;

        if(queue->cpu_burst[queue->curr_cpu] != 0) {      
            add_task(queue->next, tmp);
            tmp = NULL;
        }
    }
}

// Function to Print queue
// void print_process(Process *queue){
//     printf("Process with pid  %d ends its %d th burst at t= %d\n",queue->pid,queue->curr_cpu, global_time);
// }

// Function to add a new task to the task queue
void add_task(Process **queue, Process *new_task) {
    if (*queue == NULL || (*queue)->arrival_time > new_task->arrival_time) {
        new_task->next = *queue;
        *queue = new_task;
    }
    else {
        Process *current_task = *queue;
        while (current_task->next != NULL && current_task->next->arrival_time < new_task->arrival_time) {
            current_task = current_task->next;
        }
        new_task->next = current_task->next;
        current_task->next = new_task;
    }
}
int main() {
    read_tasks("PreEmptiveRR.txt");
    round_robin();
    print_results();
    return 0;
}
