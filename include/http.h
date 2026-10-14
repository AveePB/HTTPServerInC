#ifndef HTTP_H
#define HTTP_H

#define HTTP_VERSION_MAX_LEN 16
#define HTTP_TARGET_MAX_LEN 256
#define HTTP_METHOD_MAX_LEN 8
#define BUFFER_SIZE 1024

void handleClient(int *client_sock_fd);
void sendResourceInfo(int *client_sock_fd, const char* filename);
void sendHTML(int *client_sock_fd, const char *filename);
void send404(int *client_sock_fd);

#endif