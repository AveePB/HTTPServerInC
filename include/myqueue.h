#ifndef MYQUEUE_H
#define MYQUEUE_H
#include <stdlib.h>

typedef struct node {
    struct node *next;
    int *client_sock_fd;

} node_t;

void enqueue(int *client_sock_fd);
int *dequeue();

#endif