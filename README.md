the purpose of this programm is to simulate an eshop enviroment where the "server" is the parent process and the "client" is the child process. This is done by using the fork() functions in which the 2 processes are communicating via a pipe(). Firstly, the programm initializes the rand() seed and the catalog struct. When that is done, the child process (the "client") generates a random number indicating which product number he would like to purchase and that number is sent to the parent process (the "server"). The server processes the order, if the product is available it updates the client's total and the same process starts again. At the end of the client's final offer, if all of his products are available he is shown his final cost with the appropriate message. On the other hand, if at least one product is unavailable he encounters an error. At the end of the programm, the parent process shows: 1) the total orders 2) total successful orders 3) total failed orders 4) total profit 5) how many products were requested 6) how many products were bought 7) total amount of orders 8) how many requests were made 9) how many requests succeeded 10) how many requests failed.
