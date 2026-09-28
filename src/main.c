#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define PORT 9090
#define BUFFER_SIZE 1024

void sendHTML(int *sock_fd, const char *file_fd) {
    // Find and open html file
    FILE *html = fopen(file_fd, "r");
    if (!html) {
        perror("Could not open HTML file");
        return;
    }
    char buffer[BUFFER_SIZE] = {0};
    size_t read = 0;

    // Send header
    char *header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    send(*sock_fd, header, strlen(header), 0);

    // Send HTML file
    while ((read = fread(buffer, sizeof(char), BUFFER_SIZE, html)) > 0) {
        send(*sock_fd, buffer, read, 0);
    }

    fclose(html);
}

int main() {
    // Create endpoint for communication
    int server_sock_fd;
    if ((server_sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Could not get server socket fd");
        return -1;
    }

    // Create internet socket address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    server_addr.sin_port = htons(PORT);

    // Assign address to socket 
    if (bind(server_sock_fd, (struct  sockaddr *)&server_addr, sizeof server_addr) < 0) {
        perror("Could not bind address to socket");
        return -1;
    }

    // Start listening on given port
    if (listen(server_sock_fd, 5) < 0) {
        perror("Could not listen on server socket");
        return -1;
    }

    printf("Started listening on port %d\n", PORT);

    // Process client requests
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof client_addr;
        int *client_sock_fd = malloc(sizeof(int));

        if ((*client_sock_fd = accept(server_sock_fd, (struct  sockaddr*)&client_addr, &client_len)) < 0) {
            perror("Could not accept client");
            continue;
        }

        // Handle client
        printf("Client connected\n");
        char recBuffer[BUFFER_SIZE] = {0};
        recv(*client_sock_fd, recBuffer, BUFFER_SIZE, 0);
        printf("%s\n", recBuffer);
        sendHTML(client_sock_fd, "./static/home.html");

        // Disconnect client
        close(*client_sock_fd);
        free(client_sock_fd);
        printf("Client disconnected\n");
    }
    close(server_sock_fd);

    return 0;
}
