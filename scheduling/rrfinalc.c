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

    int max_cpu;
    int max_io;
    
    int cpu_time_left;
    struct process *next;
} Process ;

// Function to add a new task to the task queue
void add_task(Process **queue, Process *new_task) {
    new_task->max_cpu = new_task->curr_cpu;
    new_task->max_io = new_task->curr_io;
    new_task->curr_cpu = 0;
    new_task->curr_io = 0;
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
int num_process = 0;
int count = 0;

// //for reading
void read_tasks(const char* filename) {
    printf("\nINSIDE read_tasks\n");
    FILE *file = fopen(filename, "r");
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
        num_process++;
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
    while(count<num_process){
    printf("\n%d\n", count);
    
    printf("\nINSIDE round_robin()\n");
    printf("\n1\n");

    if(queue->arrival_time > global_time){
        printf("Idle from %d to %d\n", global_time, queue->arrival_time);
        global_time = queue->arrival_time;
    }

    printf("\n2\n");
    
    if(queue->cpu_burst[queue->curr_cpu] <= QUANTUM_TIME){
    printf("\n3\n");
        
        global_time += queue->cpu_burst[queue->curr_cpu];
        printf("\n3.1\n");
        printf("\n%d|n", queue->max_io);

        if(queue->curr_io <= queue->max_io){
        queue->arrival_time = global_time + queue->io_burst[queue -> curr_io];
        printf("\n3.2\n");
        }

        queue->curr_cpu ++;
        printf("\n3.3\n");

        queue->curr_io ++;
        printf("\n3.4\n");
       

        Process *tmp = queue;
        tmp->next = NULL;

        if(queue->next!=NULL){
            queue = queue->next;
        }
         
        printf("\n3.5\n");

        if(queue->cpu_burst[queue->curr_cpu] != 0) {  
            printf("\n3.6\n");
            printf("Process with pid  %d ends its %d th burst at t= %d\n",queue->pid,queue->curr_cpu, global_time);    
            add_task(&queue, tmp);
           }

        else{
            printf("\n3.7\n");
            count++;
            printf("Process with pid %d ends at t= %d\n",queue->pid,global_time); 

            if (count == num_process){
                printf("End of schedule at time = %d\n", global_time);
            }
        }
    }
    else{
    printf("\n4\n");

        global_time += QUANTUM_TIME;
        queue->arrival_time = global_time;

        printf("\n4.1\n");    

        queue->cpu_burst[queue->curr_cpu] -= QUANTUM_TIME;
        printf("\n4.2\n");    

        Process *tmp = queue;
        tmp->next = NULL;
        printf("\n4.3\n");    

        if(queue->next != NULL){
            printf("\n queue is null\n");
            queue = queue->next;
        }
        
        printf("\n4.4\n");   
        printf("\n%d\n", queue->curr_io); 
        printf("\n%d\n", queue->curr_cpu);
        

         add_task(&queue, tmp);
         tmp = NULL;
    
    }
    printf("\n5\n");
    
}

}

// Function to Print Queue
void print_processes(Process *queue){
    Process* temp = queue;
    while(temp!=NULL){
        printf("\nProcess Id : %d, Arrival Time : %d\n", temp->pid, temp->arrival_time);
        printf("CPU Bursts for process %d : ", temp->pid);
        for(int i = 0; i<temp->curr_cpu; ++i){
            printf("%d ", temp->cpu_burst[i]);
        }
        printf("\nIO Bursts for process %d : ", temp->pid);
        for(int i = 0; i<temp->curr_io; ++i){
            printf("%d ", temp->io_burst[i]);
        }
        printf("\n");
        temp = temp->next;
    }
}


int main(int argc, char *argv[]) {
    if(argc!=2){
        printf("Invalid arguments, run command in the format :\n%s filename.txt\n", argv[0]);
        return 1;
    }
    printf("\nINSIDE main\n");
    read_tasks(argv[1]);
    print_processes(queue);
    round_robin();
    // print_results();
    return 0;
}
