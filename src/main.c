#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "http.h"
#include "myqueue.h"

#define THREAD_POOL_SIZE 20
#define PORT "3490"
#define BACKLOG 100

pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER;

void *threadFunction(void *arg);

int main(void) {
    // Initialize informational struct
    pthread_t thread_pool[THREAD_POOL_SIZE];
    struct addrinfo hints, *res;
    int server_sock_fd;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE;     // Bind to specific local IP address
    getaddrinfo(NULL, PORT, &hints, &res);

    // Initialize thread pool
    for (int i=0; i<THREAD_POOL_SIZE; i++)
        pthread_create(&thread_pool[i], NULL, threadFunction, NULL);

    // Create server socket
    if ((server_sock_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
        perror("Failed to get socket file descriptor");
        return -1;
    }
    printf("Server got socket file descriptor!\n");

    // Associate socket with port
    if (bind(server_sock_fd, res->ai_addr, res->ai_addrlen) < 0) {
        perror("Failed to bind socket to port");
        return -1;
    }
    printf("Server socket bound to port\n");

    // Listen for incoming connections
    if (listen(server_sock_fd, BACKLOG) < 0) {
        perror("Failed to listen on port");
        return -1;
    } 
    printf("Server started listening for connections!\n");

    struct sockaddr_storage client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int client_sock_fd;

    while(1) {
        // Accept incoming connection
        if ((client_sock_fd = accept(server_sock_fd, (struct sockaddr *)&client_addr, &client_addr_size)) < 0) {
            perror("Failed to establish stable connection with client");
            continue;
        }

        // Initialize space in memory for client socket file descriptor
        int *pclient_sock_fd = malloc(sizeof(int));
        *pclient_sock_fd = client_sock_fd;

        // Put client onto the queue
        pthread_mutex_lock(&queue_mutex);
        enqueue(pclient_sock_fd);
        pthread_cond_signal(&condition_var);
        pthread_mutex_unlock(&queue_mutex);
    }
    close(server_sock_fd);
    return 0;
}

void *threadFunction(void *arg) {
    while (1) {
        // Grab client from queue
        pthread_mutex_lock(&queue_mutex);

        int *pclient_sock_fd;
        if ((pclient_sock_fd = dequeue()) == NULL)
            pthread_cond_wait(&condition_var, &queue_mutex);
            
        pthread_mutex_unlock(&queue_mutex);

        // There is client to handle
        if (pclient_sock_fd != NULL) {
            printf("Successfully established stable connection with client!\n");
            handleClient(pclient_sock_fd);
            printf("Client handled, connection closed.\n");
        }
    }
}
