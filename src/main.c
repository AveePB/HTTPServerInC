#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <unistd.h>

#include "http.h"

#define PORT "3490"
#define BACKLOG 10

int main(void) {
    // Initialize informational struct
    struct addrinfo hints, *res;
    int server_sock_fd;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE;     // Bind to specific local IP address
    getaddrinfo(NULL, PORT, &hints, &res);

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

        printf("Successfully established stable connection with client!\n");
        handleClient(&client_sock_fd);
        printf("Client handled, connection closed.\n");
    }
    close(server_sock_fd);
    return 0;
}