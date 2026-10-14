#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "http.h"

void handleClient(int *client_sock_fd) {
    char recv_buffer[BUFFER_SIZE] = {0};
    // Error during reading client request 
    if (recv(*client_sock_fd, recv_buffer, BUFFER_SIZE, 0) < 0) {
        printf("Server cannot process client request!\n");
    }

    // Request read, start processing 
    else {
        char http_method[HTTP_METHOD_MAX_LEN];
        char http_target[HTTP_TARGET_MAX_LEN];
        char http_version[HTTP_VERSION_MAX_LEN];

        // Requests lacks needed informations 
        if (sscanf(recv_buffer, "%[a-zA-Z] %[a-zA-Z/.] %[0-9HTPShtps/.]", http_method, http_target, http_version) != 3) {
            printf("Client request is not a valid one!\n");
        }

        // Client request is valid
        else {
            printf("Successfully parsed client request: %s %s %s\n", http_method, http_target, http_version);
            
            if (strcmp(http_method, "GET") == 0) {
                if (strcmp(http_target, "/") == 0) 
                    sendHTML(client_sock_fd, "./static/home.html");
                else if (strcmp(http_target, "/about") == 0)
                    sendHTML(client_sock_fd, "./static/about_me.html");
                else
                    send404(client_sock_fd);
            }

            else if (strcmp(http_method, "HEAD") == 0) {
                if (strcmp(http_target, "/") == 0) 
                    sendResourceInfo(client_sock_fd, "./static/home.html");
                else if (strcmp(http_target, "/about") == 0)
                    sendResourceInfo(client_sock_fd, "./static/about_me.html");
                else
                    send404(client_sock_fd);
            }

            else if (strcmp(http_method, "OPTIONS") == 0) {
                char *header_204 = "HTTP/1.1 204 No Content\r\nAllow: OPTIONS, GET, HEAD\r\n\r\n";
                send(*client_sock_fd, header_204, strlen(header_204), 0);
            }

            // 405 Method Not Allowed
            else if (strcmp(http_method, "POST") == 0 ||
                    strcmp(http_method, "PUT") == 0 ||
                    strcmp(http_method, "DELETE") == 0 ||
                    strcmp(http_method, "PATCH") == 0 ||
                    strcmp(http_method, "TRACE") == 0 ||
                    strcmp(http_method, "CONNECT") == 0) {
                char *header_405 = "HTTP/1.1 405 Method Not Allowed\r\n\r\n";
                send(*client_sock_fd, header_405, strlen(header_405), 0);
            }

            // 400 Bad Request 
            else {
                char *header_400 = "HTTP/1.1 400 Bad Request\r\n\r\n";
                send(*client_sock_fd, header_400, strlen(header_400), 0);
            }
        }
    }
    close(*client_sock_fd);
}

void sendResourceInfo(int *client_sock_fd, const char* filename) {
    // Open file
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Could not open HTML file");
        send404(client_sock_fd);
        return;
    }

    // Move pointer to the end of file
    if (fseek(file, 0L, SEEK_END) != 0) {
        perror("Could not open identify sie of file");
        fclose(file);

        char *header_500 = "HTTP/1.1 500 Internal Server Error\r\n\r\n";
        send(*client_sock_fd, header_500, strlen(header_500), 0);
    }

    int size = (int) ftell(file);
    fclose(file);

    // Create response 
    char header_200[128];
    snprintf(header_200, sizeof(header_200), "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n", size);
    send(*client_sock_fd, header_200, strlen(header_200), 0);
}

void sendHTML(int *client_sock_fd, const char *filename) {
    // Open file
    FILE *html_file = fopen(filename, "r");
    if (!html_file) {
        perror("Could not open HTML file");
        send404(client_sock_fd);
        return;
    }

    // Send header 
    char *header_200 = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    send(*client_sock_fd, header_200, strlen(header_200), 0);

    // Send HTML file
    size_t read = 0;
    char buffer[BUFFER_SIZE] = {0};
    while ((read = fread(buffer, sizeof(char), BUFFER_SIZE, html_file)) > 0) {
        send(*client_sock_fd, buffer, read, 0);
    }
    fclose(html_file);
}

void send404(int *client_sock_fd) {
    char *header_404 = "HTTP/1.1 404 Not Found\r\n\r\n";
    send(*client_sock_fd, header_404, strlen(header_404), 0);
}
