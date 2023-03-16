typedef struct Node {
    unsigned int value;
    struct Node *next;
} Node;


void push(Node **h, unsigned int data);
int pop(Node **h);
void deleteList(Node **head);
