#include <stdio.h>
#include "list.h"

/*********************************************************************
 *
 * @purpose  Push element data to list h
 *
 * @param Node **h              head of list
 *
 * @param unsigned int data     data
 *
 * @return void
 *
 * @note This function push element "data" to list "h"
 *
 * @end
 *
 *********************************************************************/
void push(Node **h, unsigned int data)
{
    Node *tmp = (Node*) malloc(sizeof(Node));
    tmp->value = data;
    tmp->next = (*h);
    (*h) = tmp;
}

/*********************************************************************
 *
 * @purpose  Pop element from list h
 *
 * @param Node **h              head of list
 *
 * @return unsigned int         data of element
 *
 * @note This function pop element from list "h"
 *
 * @end
 *
 *********************************************************************/
unsigned int pop(Node **h) {
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

/*********************************************************************
 *
 * @purpose  Delete List and free memory
 *
 * @param Node **h              head of list
 *
 * @return void
 *
 * @note This function deletes list "h" and free memory
 *
 * @end
 *
 *********************************************************************/
void deleteList(Node **h) {
    Node* prev = NULL;
    while ((*h)->next) {
        prev = (*h);
        (*h) = (*h)->next;
        free(prev);
    }
    free(*h);
}
