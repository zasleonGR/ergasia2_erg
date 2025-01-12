//Header files used

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include "eshopFunctions.h"


//Constast variables

#define CLIENTS 5       
#define MAX_ITEMS 20
#define ORDERS 10

//Global variables
double total_profit = 0.0;
int sucs_orders = 0;
int failed_orders = 0;
char Buffer[256] = { 0 };
int random_item;
double total_price; 
int error_flag;
int sucs_request;
int failed_request;


struct Product {
    char description[100];
    double price;
    int item_count;
};

struct Product catalog[MAX_ITEMS]; //initialization of the catalog


void initialize_catalog() {
    for (int i = 0; i < MAX_ITEMS; i++) {
        catalog[i].item_count = 2;
    }
}

void initialize_prices() {
    for (int i = 0; i < MAX_ITEMS; i++) {
        catalog[i].price = rand() % (1000 + 1);
    }
}

void process_order(int read_fd, int write_fd) {

    int random_item = rand() % (MAX_ITEMS);
    int item_number = random_item;
    read(read_fd, &item_number, sizeof(item_number)); // Read item number from pipe

    sleep(0.5);

    if (catalog[item_number].item_count > 0) {
        catalog[item_number].item_count--; 
        total_price = catalog[item_number].price; 
        error_flag = 0; // Order successful
        sucs_orders++;
    } else {
        error_flag = 1; // Order failed
        failed_orders++;
    }


    write(write_fd, &total_price, sizeof(total_price));// Write results back to parent
    write(write_fd, &error_flag, sizeof(error_flag));
}

void print_result(int Client, double Final_price, int error_flag) {
    printf("Client %d:", Client);
    if (error_flag == 0) {
        printf(" Purchase complete, your total is: %.2f euro.\n", Final_price);
        sucs_request++;
        total_profit += Final_price; // Add to total_profit for this client
    } else {
        printf(" At least 1 product was unavailable, request failed.\n");
        failed_request++;
    }
}


void end_results_print(int s_o,int f_o,double t_p,int s_r,int f_r){
    printf("\nTotal orders: %d\n", sucs_orders + failed_orders);
    printf("Total successful orders: %d\n", sucs_orders);
    printf("Total failed orders: %d\n", failed_orders);
    printf("Total profit: %.2f euro.\n\n", total_profit);

    printf("%d products were requested,where %d products were bought,totaling %.2f euros.\n", sucs_orders + failed_orders, sucs_orders, total_profit);
    printf("%d requests were made,where %d succeeded and %d failed\n", sucs_request+failed_request, sucs_request, failed_request);
}


int main() {
    srand(time(NULL)); // Seed the random number generator once at the start

    int order_pipe[ORDERS][2];  // We need 2 (4) pipes for each process
    int result_pipe[ORDERS][2];

    initialize_catalog(); //We initalize the catalog (and the prices of the items etc)
    initialize_prices();

    for (int y = 0; y < CLIENTS; y++) {

        total_price = 0.0; // Reset total_price for each client
        error_flag = 0; // Reset error_flag for each client

        for (int i = 0; i < ORDERS; i++) {

            if (pipe(order_pipe[i]) == -1) {
                fprintf(stderr, "Pipe1 failed");
                return 1;
            }

            if (pipe(result_pipe[i]) == -1) {
                fprintf(stderr, "Pipe2 failed");
                return 1;
            }

            pid_t p = fork();       //We use fork
            if (p < 0) {
                perror("fork fail");
                exit(1);
            } else if (p == 0) { // Child process
                
                close(order_pipe[i][0]);  //We close the read end of the pipe
                close(result_pipe[i][1]);    //We close the write end of the pipe
                sleep(0.5);

                random_item = rand() % (MAX_ITEMS); // Generate random item
               
                
                write(order_pipe[i][1], &random_item, sizeof(random_item)); //Sends order to the child process
                close(order_pipe[i][0]);
                close(order_pipe[i][1]);
                exit(0);
                
            } else {  // Parent process
                
                close(order_pipe[i][1]);  
                close(result_pipe[i][0]); 
                sleep(0.5);

                
                read(order_pipe[i][0], Buffer, sizeof(Buffer)); //Reads the answere from the child process
                process_order(order_pipe[i][1], result_pipe[i][0]);
               



                close(result_pipe[i][1]);
                close(order_pipe[i][0]);

            }
        }
        sleep(1);
        print_result(y + 1, total_price, error_flag); 
    }


    end_results_print(sucs_orders, failed_orders, total_profit, sucs_request, failed_request);
    


    for (int i = 0; i < CLIENTS; i++) {
        wait(NULL);
    }

    return 0;
}
