#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define CLIENTS 2
#define MAX_ITEMS 20


double total_profit =0.0;
int total_orders =0;
int sucs_orders =0;
int failed_orders =0;
char Buffer[256] = { 0 };

struct Product{
  char description[100];
  double price;
  int item_count;
};


struct Product catalog[20];


void initialize_catalog() {
    for (int i = 0; i < MAX_ITEMS; i++) {
      catalog[i].item_count = 2;
    }
}

void process_order(int read_fd, int write_fd){
  //
}

void print_result (int Client, double Final_price ,int error_flag ){
  
  printf("Client %d:",Client);
  if (error_flag == 0)
    printf("Purchase complete, your total is : %.2f euro.\n", Final_price);
  else 
    printf("At least 1 product was unavaiable,request failed.\n");
}


int main(){

  
  
  char input_str[100];
  int order_pipe[CLIENTS][2];  //We need 2 (4) pipes for each process
  int result_pipe[CLIENTS][2];

  
  initialize_catalog();
for (int i = 0; i < CLIENTS; i++) {
  
    // Create pipes for each client
  
    if (pipe(order_pipe[i]) == -1) {
        fprintf(stderr, "Pipe1 failed");
        return 1;
    }
  
    if (pipe(result_pipe[i]) == -1) {
      fprintf(stderr, "Pipe2 failed");
      return 1;
      }
    
      pid_t p = fork();
    if (p < 0) {
  
      perror("fork fail");
      exit(1);
  
     }else if (p == 0) { // Child process
  
        close(order_pipe[i][1]); 
  
        read(order_pipe[i][0], Buffer, sizeof(Buffer)); 
        printf("Sent client's %dth order\n", i + 1);
  
        close(order_pipe[i][0]);
        exit(0); 
      }else {  
  
         close(order_pipe[i][0]); 
  
         write(order_pipe[i][1], "Client orders item\n", 20); 
  
        printf("Client orders item %d\n",i+1);
        close(order_pipe[i][1]); 
      }
  
}
    
  for (int i = 0; i < CLIENTS; i++) {
    wait(NULL);
  }

  return 0;
}

