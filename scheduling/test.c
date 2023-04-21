#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_NUMBERS 100
#define MAX_LINES 100


typedef struct process {
    int pid;
    int arrival_time;
    int cpu_burst[10];
    int io_burst[10];
    int curr_cpu;
    int curr_io;
    int cpu_time_left;
    struct process *next;
} Process;

int main() {
    FILE *file = fopen("tasks.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    char numbers[MAX_NUMBERS]; 
    int flag, i, j, num_lines = 0;
    // Read the tasks from the file and add them to the task queue
    
    // Read the file line by line
    while (fgets(numbers, MAX_NUMBERS * sizeof(int), file)) {
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

        printf("\nProcess Id : %d, Arrival Time : %d\n", new_process->pid, new_process->arrival_time);
        printf("CPU Bursts for process %d : ", new_process->pid);
        for(int i = 0; i<new_process->curr_cpu; ++i){
            printf("%d ", new_process->cpu_burst[i]);
        }
        printf("\nIO Bursts for process %d : ", new_process->pid);
        for(int i = 0; i<new_process->curr_io; ++i){
            printf("%d ", new_process->io_burst[i]);
        }
        printf("\n");

    }





    // char ch = getc(file);
    // printf("lol %d\n", ch-48);
    // while (!feof(file)) {
    //     char buf[1000];
    //     printf("1\n");
    //     int buf_index = 0;
    //     printf("2\n");
    //     while(ch != '\n'){
    //         printf("3\n");
    //         buf[buf_index] = ch;
    //         printf("4\n");
    //         buf_index++;
    //         printf("5\n");
    //         // putchar(ch);
    //         printf("6\n");
    //         printf("token %d", ch-48);
    //         printf("7\n");
    //         ch = getc(file);
    //     }
    //     const char* delim = " ";
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
    //             printf("%d", atoi(token));
    //             new_process->curr_cpu++;
    //         }else{
    //             new_process->io_burst[new_process->curr_io] = atoi(token);
    //             new_process->curr_io++;
    //         }
    //         token  = strtok(NULL, delim);
    //         flag = !flag;
    //     }
    //     // add_task(&queue, new_process);
    // }


    // FILE *fp;
    // char filename[] = "tasks.txt";
    // char buf[100];
    // Process *head = NULL, *curr, *temp;
    // int i, j, task_num, arrival_time;

    // fp = fopen(filename, "r");

    // while (fgets(buf, sizeof(buf), fp) != NULL) {
    //     curr = (Process*)malloc(sizeof(Process));
    //     sscanf(buf, "%d %d", &task_num, &arrival_time);
    //     curr->arrival_time = arrival_time;
        
    //     for (i = 0, j = 2; i < 10 && j < 7; i++, j += 2) {
    //         sscanf(buf + (j * 2), "%d %d", &curr->cpu_burst[i], &curr->io_burst[i]);
    //     }
    //     curr->next = NULL;

    //     if (head == NULL) {
    //         head = curr;
    //     } else {
    //         temp = head;
    //         while (temp->next != NULL) {
    //             temp = temp->next;
    //         }
    //         temp->next = curr;
    //     }
    // }

    // fclose(fp);

    // // Print the values of the processes
    // curr = head;
    // while (curr != NULL) {
    //     printf("Task %d, Arrival Time: %d, CPU Burst: ", task_num, curr->arrival_time);
    //     for (i = 0; i < 10; i++) {
    //         printf("%d ", curr->cpu_burst[i]);
    //     }
    //     printf(", IO Burst: ");
    //     for (i = 0; i < 10; i++) {
    //         printf("%d ", curr->io_burst[i]);
    //     }
    //     printf("\n");
    //     curr = curr->next;
    // }

    // // Free the allocated memory
    // curr = head;
    // while (curr != NULL) {
    //     temp = curr;
    //     curr = curr->next;
    //     free(temp);
    // }

    return 0;
}
