#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BURSTS 100
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

// Function to add a new task to the task queue
void add_task(Process **queue, Process *new_task) {
    printf("\nINSIDE add_tasks\n");
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

// Global variables
Process *queue = NULL;
int global_time = 0;
int num_process;
int count = 0;

// //for reading
void read_tasks(const char* filename) {
    printf("\nINSIDE read_tasks\n");
    FILE *file = fopen("tasks.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    char numbers[MAX_BURSTS]; 
    int flag, i, j, num_lines = 0;
    // Read the tasks from the file and add them to the task queue
    
    // Read the file line by line
    while (fgets(numbers, MAX_BURSTS * sizeof(int), file)) {
        Process *new_process = (Process *)(malloc(sizeof(Process)));
        // Split the line into numbers
        char *token = strtok(numbers, " ");
        j = 0;
        flag = 1;
        
        while (token != NULL) {
            // Convert the token to an integer and store it in the array
            if(flag == 1){
                new_process->pid = atoi(token);
                flag++;
            }
            else if(flag == 2){
                new_process->arrival_time = atoi(token);
                flag++;
            }
            else{
                if(flag%2 != 0){
                    new_process->cpu_burst[new_process->curr_cpu] = atoi(token);
                    new_process->curr_cpu++;
                    flag++;
                }
                else{
                    new_process->io_burst[new_process->curr_io] = atoi(token);
                    new_process->curr_io++;
                    flag++;
                }
            }
            // arrays[num_lines][j] = atoi(token);
            j++;
            token = strtok(NULL, " ");
        }
        num_lines++;
        add_task(&queue, new_process);

    }



    // FILE *file = fopen(filename, "r");
    // if (file == NULL) {
    //     perror("Error opening file");
    //     exit(EXIT_FAILURE);
    // }
    
    // // Read the tasks from the file and add them to the task queue
   
    // while (1) {
    //     char buf[1000];
    //     char ch = getc(file);
    //     int buf_index = 0;
    //     if(ch == EOF)break;
    //     while(ch != '\n'){
    //         buf[buf_index] = ch;
    //         ch = getc(file);
    //     }
    //     char * delim = " ";
    //     char *token = strtok(buf,delim);

    //     Process *new_process = (Process *)(malloc(sizeof(Process)));
    //     new_process->curr_cpu = 0;
    //     new_process->curr_io = 0;

    //     new_process->pid = atoi(token);
    //     token  = strtok(NULL, delim);

    //     new_process->arrival_time = atoi(token);
    //     token  = strtok(NULL, delim);
    //     int flag = 1;
    //     while(token != NULL){
    //         if(flag){
    //             new_process->cpu_burst[new_process->curr_cpu] = atoi(token);
    //             new_process->curr_cpu++;
    //         }else{
    //             new_process->io_burst[new_process->curr_io] = atoi(token);
    //             new_process->curr_io++;
    //         }
    //         token  = strtok(NULL, delim);
    //         flag = !flag;
    //     }
    //     add_task(&queue, new_process);
    // }
}

void round_robin(){
    printf("\nINSIDE round_robin()\n");
 
    if(queue->arrival_time > global_time){
        global_time = queue->arrival_time;
    }

    if(queue->cpu_burst[queue->curr_cpu] <= QUANTUM_TIME){
        global_time += queue->cpu_burst[queue->curr_cpu];
        queue->arrival_time = global_time + queue->io_burst[queue -> curr_io];

        queue->curr_cpu ++;
        queue->curr_io ++;
       

        Process *tmp = queue;
        tmp->next = NULL;

        queue = queue->next; 

        if(queue->cpu_burst[queue->curr_cpu] != 0) {  
            printf("Process with pid  %d ends its %d th burst at t= %d\n",queue->pid,queue->curr_cpu, global_time);    
            add_task(&queue, tmp);
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
            add_task(&queue, tmp);
            tmp = NULL;
        }
    }
}

// Function to Print queue
void print_process(Process *queue){
    printf("Process with pid  %d ends its %d th burst at t= %d\n",queue->pid,queue->curr_cpu, global_time);
}


int main() {
    printf("\nINSIDE main\n");
    read_tasks("tasks.txt");
    print_process(queue);
    round_robin();
    // print_results();
    return 0;
}