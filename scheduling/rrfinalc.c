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
  struct process * next;

} Process;

Process * queue = NULL;

// Function to Print Queue
void print_processes(Process *Queue) {
  Process* temp = Queue;
  while (temp != NULL) {
    printf("\nProcess Id : %d, Arrival Time : %d\n", temp -> pid, temp -> arrival_time);
    printf("CPU Bursts for process %d : ", temp -> pid);
    for (int i = 0; i < temp -> max_cpu; ++i) {
      printf("%d ", temp -> cpu_burst[i]);
    }
    printf("\nIO Bursts for process %d : ", temp -> pid);
    for (int i = 0; i < temp -> max_io; ++i) {
      printf("%d ", temp -> io_burst[i]);
    }
    printf("\n");
    temp = temp -> next;
  }
}

// Function to add a new task to the task queue
void add_task(Process ** queue, Process * new_task) {

    /*

        if (empty) assign in beginning

        if (middle)
        
        if (tail)else assign in middle/end


    */

  printf("\nINSIDE add_tasks\n");

  if ( *queue == NULL || ( * queue) -> arrival_time > new_task -> arrival_time) {

    new_task -> next = *queue;
    *queue = new_task;

  } else {
    Process *current_task = *queue;

    while (current_task -> next != NULL && current_task -> next -> arrival_time < new_task -> arrival_time) {
      current_task = current_task -> next;
    }
    
    new_task -> next = current_task -> next;
    current_task -> next = new_task;
  }
}

// Global variables

int global_time = 0;
int num_process = 0;
int count = 0;

// //for reading
void read_tasks(const char * filename) {
  printf("\nINSIDE read_tasks\n");
  FILE * file = fopen(filename, "r");
  if (file == NULL) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }
  char numbers[MAX_BURSTS];
  int flag;
  // Read the tasks from the file and add them to the task queue

  // Read the file line by line
  while (fgets(numbers, MAX_BURSTS * sizeof(int), file)) {
    Process * new_process = (Process * )(malloc(sizeof(Process)));

    new_process -> curr_cpu = 0;
    new_process -> curr_io = 0;
    new_process -> max_cpu = 0;
    new_process -> max_io = 0;
    new_process -> next = NULL;
    // Split the line into numbers
    char * token = strtok(numbers, " ");
    flag = 1;

    while (token != NULL) {
      // Convert the token to an integer and store it in the array
      if (flag == 1) {
        new_process -> pid = atoi(token);
        flag++;
      } else if (flag == 2) {
        new_process -> arrival_time = atoi(token);
        flag++;
      } else {
        if (flag % 2 != 0) {
          new_process -> cpu_burst[new_process -> curr_cpu] = atoi(token);
          new_process -> curr_cpu++;
          flag++;
        } else {
          new_process -> io_burst[new_process -> curr_io] = atoi(token);
          new_process -> curr_io++;
          flag++;
        }
      }
      
      token = strtok(NULL, " ");
    }
    num_process++;
    new_process -> max_cpu = new_process -> curr_cpu;
    new_process -> max_io = new_process -> curr_io;

    new_process -> curr_cpu = 0;
    new_process -> curr_io = 0;

    add_task( & queue, new_process);

  }
}

void round_robin() {

  printf("\nINSIDE ROUND ROBIN\n");

//   printf("count = %d\n", count);  
//   printf("numoricess = %d\n", num_process);  
//   print_processes(queue);

  while (count < num_process) {
    Process* temp = queue;

    
    if (queue -> next != NULL) {
      queue = queue -> next;
    }
    
    

    if (temp -> arrival_time > global_time) {
      printf("Idle from %d to %d\n", global_time, temp -> arrival_time);
      global_time = temp -> arrival_time;
    }

    if (temp -> cpu_burst[temp -> curr_cpu] <= QUANTUM_TIME) {
      

      global_time += temp -> cpu_burst[temp -> curr_cpu];
      
      

      if (temp -> curr_io <= temp -> max_io) {
        temp -> arrival_time = global_time + temp -> io_burst[temp -> curr_io];
        
      }

      temp -> curr_cpu++;
      

      temp -> curr_io++;
      

      

      if (temp -> cpu_burst[temp -> curr_cpu] != 0) {
        
        printf("Process with pid  %d ends its %d th burst at t= %d\n", temp -> pid, temp -> curr_cpu, global_time);
        add_task( & queue, temp);
      } else {
        
        count++;
        printf("Process with pid %d ends at t= %d\n", temp -> pid, global_time);

        if (count == num_process) {
          printf("End of schedule at time = %d\n", global_time);
        }
      }
    } else {
      

      global_time += QUANTUM_TIME;
      temp -> arrival_time = global_time;

      

      temp -> cpu_burst[temp -> curr_cpu] -= QUANTUM_TIME;
      

      Process * tmp = temp;
      tmp -> next = NULL;
      

      
      
      

      add_task( & queue, tmp);
      tmp = NULL;

    }
    

  }
  
  

}

int main(int argc, char * argv[]) {
  if (argc != 2) {
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
