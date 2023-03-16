/*********************************************************************
This module implements a minimal set of function for working with list
********************************************************************/

// Structure of list node
typedef struct Node {
    unsigned int value;
    struct Node *next;
} Node;

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
void push(Node **h, unsigned int data);

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
unsigned int pop(Node **h);

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
void deleteList(Node **head);
