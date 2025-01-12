//header files used.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

//contrast variables.
#define CLIENTS 5       
#define MAX_ITEMS 20
#define ORDERS 10

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
