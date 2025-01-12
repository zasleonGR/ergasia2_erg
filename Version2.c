//Header files used

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include "eshopFunctions.h"


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
