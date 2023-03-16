#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"

void push(Node **h, unsigned int data)
{
    Node *tmp = (Node*) malloc(sizeof(Node));
    tmp->value = data;
    tmp->next = (*h);
    (*h) = tmp;
}


int pop(Node **h) {
    Node* prev = NULL;
    int val;
    if (h == NULL) {
        return -1;
    }
    prev = (*h);
    val = prev->value;
    (*h) = (*h)->next;
    free(prev);
    return val;
}

void deleteList(Node **h) {
    Node* prev = NULL;
    while ((*h)->next) {
        prev = (*h);
        (*h) = (*h)->next;
        free(prev);
    }
    free(*h);
}

