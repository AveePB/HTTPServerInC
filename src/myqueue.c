#include "myqueue.h"

node_t *head = NULL;
node_t *tail = NULL;

void enqueue(int *client_sock_fd) {
    // Create new node
    node_t *new_node = malloc(sizeof(node_t));
    new_node->client_sock_fd = client_sock_fd;
    new_node->next = NULL;

    // Add new element to queue
    if (tail == NULL)
        head = new_node;
    else
        tail->next = new_node;

    tail = new_node;
}

int *dequeue() {
    // Queue is empty
    if (head == NULL) return NULL;

    // Move nodes
    int *res = head->client_sock_fd;
    node_t *temp_node = head;
    head = head->next;
    if (head == NULL) tail = NULL;
    free(temp_node);

    return res;
}