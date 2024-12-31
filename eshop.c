#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define CLIENTS 5
#define MAX_ITEMS 20
#define ORDERS 10

double total_profit = 0.0;
int sucs_orders = 0;
int failed_orders = 0;
char Buffer[256] = { 0 };
int random_item;
double total_price; // Removed the global total_price
int error_flag;

struct Product {
    char description[100];
    double price;
    int item_count;
};

struct Product catalog[MAX_ITEMS]; // Adjusted to use MAX_ITEMS

void initialize_catalog() {
    for (int i = 0; i < MAX_ITEMS; i++) {
        catalog[i].item_count = 2;
    }
}

void initialize_prices() {
    for (int i = 0; i < MAX_ITEMS; i++) {
        catalog[i].price = rand() % (100 + 1);
    }
}

void process_order(int read_fd, int write_fd, int item_number) {
    if (catalog[item_number].item_count > 0) {
        error_flag = 0;
        catalog[item_number].item_count--;
        total_profit += catalog[item_number].price; // Only add to total_profit on success
        sucs_orders++;
        total_price += catalog[item_number].price; // Add to total_price for this client
        printf("%.2f\n", total_price);
    } else {
        error_flag = 1; // Set error flag for failed order
        failed_orders++;
        // Do not modify total_profit here
    }
}

void print_result(int Client, double Final_price, int error_flag) {
    printf("Client %d:", Client);
    if (error_flag == 0) {
        printf(" Purchase complete, your total is: %.2f euro.\n", Final_price);
    } else {
        printf(" At least 1 product was unavailable, request failed.\n");
    }
}

int main() {
    srand(time(NULL)); // Seed the random number generator once at the start

    char input_str[100];
    int order_pipe[ORDERS][2];  // We need 2 (4) pipes for each process
    int result_pipe[ORDERS][2];

    initialize_catalog();
    initialize_prices();

    for (int y = 0; y < CLIENTS; y++) {
        printf("CLIENT %d\n", y + 1);
        total_price = 0.0; // Reset total_price for each client
        error_flag = 0; // Reset error_flag for each client

        for (int i = 0; i < ORDERS; i++) {
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
            } else if (p == 0) { // Child process
                close(order_pipe[i][1]);
                sleep(0.5);
                read(order_pipe[i][0], Buffer, sizeof(Buffer));
                close(order_pipe[i][0]);
                exit(0);
            } else {  // Parent process
                close(order_pipe[i][0]);
                sleep(0.5);
                write(order_pipe[i][1], "Client orders item\n", 20);

                random_item = rand() % (MAX_ITEMS); // Generate random item
                printf("Client orders item %d\n", random_item);
                process_order(order_pipe[i][1], result_pipe[i][0], random_item);
                close(order_pipe[i][1]);
                if (error_flag == 1) {
                    break; // Exit the loop if there was an error in the order
                }
            }
        }
        sleep(1);
        print_result(y + 1, total_price, error_flag); 
    }

    printf("Total orders: %d\n", sucs_orders + failed_orders);
    printf("Total successful orders: %d\n", sucs_orders);
    printf("Total failed orders: %d\n", failed_orders);
    printf("Total profit: %.2f euro.\n", total_profit);

    for (int i = 0; i < CLIENTS; i++) {
        wait(NULL);
    }

    return 0;
}

