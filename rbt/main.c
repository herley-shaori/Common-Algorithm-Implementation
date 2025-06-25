#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 100
#define MAX_LINE 1024

// Color enumeration for nodes
typedef enum { RED, BLACK } Color;

// Node structure for Red-Black Tree
typedef struct Node {
    int key;
    Color color;
    int visited; // mark node as visited during traversal
    struct Node *left, *right, *parent;
} Node;

// Forward declaration for printing tree
void printTree(Node* root, int space);

// Queue for level order traversal
typedef struct QNode {
    Node* node;
    struct QNode* next;
} QNode;

typedef struct {
    QNode* front;
    QNode* rear;
} Queue;

void enqueue(Queue* q, Node* node) {
    QNode* n = (QNode*)malloc(sizeof(QNode));
    n->node = node;
    n->next = NULL;
    if (q->rear) q->rear->next = n;
    else q->front = n;
    q->rear = n;
}

Node* dequeue_q(Queue* q) {
    if (!q->front) return NULL;
    QNode* tmp = q->front;
    Node* node = tmp->node;
    q->front = tmp->next;
    if (!q->front) q->rear = NULL;
    free(tmp);
    return node;
}

int isEmpty(Queue* q) { return q->front == NULL; }

void printQueue(Queue* q) {
    QNode* cur = q->front;
    printf("Queue: ");
    if (!cur) { printf("[empty]\n"); return; }
    while (cur) {
        printf("[ %d(%c) ]", cur->node->key, cur->node->color==RED?'R':'B');
        if (cur->next) printf(" -> ");
        cur = cur->next;
    }
    printf("\n");
}

// Red-Black Tree utility functions
Node* newNode(int key) {
    Node* n = (Node*)malloc(sizeof(Node));
    n->key = key;
    n->color = RED;
    n->visited = 0;
    n->left = n->right = n->parent = NULL;
    return n;
}

void leftRotate(Node** root, Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left) y->left->parent = x;
    y->parent = x->parent;
    if (!x->parent) *root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void rightRotate(Node** root, Node* x) {
    Node* y = x->left;
    x->left = y->right;
    if (y->right) y->right->parent = x;
    y->parent = x->parent;
    if (!x->parent) *root = y;
    else if (x == x->parent->right) x->parent->right = y;
    else x->parent->left = y;
    y->right = x;
    x->parent = y;
}

void insertFixup(Node** root, Node* z) {
    while (z->parent && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(root, z->parent->parent);
            }
        } else {
            Node* y = z->parent->parent->left;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(root, z->parent->parent);
            }
        }
    }
    (*root)->color = BLACK;
}

void rbInsert(Node** root, int key) {
    Node* z = newNode(key);
    Node* y = NULL;
    Node* x = *root;
    while (x) {
        y = x;
        if (z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    z->parent = y;
    if (!y)
        *root = z;
    else if (z->key < y->key)
        y->left = z;
    else
        y->right = z;
    insertFixup(root, z);
}

// Print the tree sideways with colors
void printTree(Node* root, int space) {
    if (!root) return;
    space += 6;
    printTree(root->right, space);
    printf("\n");
    for (int i = 6; i < space; i++)
        printf(" ");
    printf("[%d(%c)]\n", root->key, root->color == RED ? 'R' : 'B');
    printTree(root->left, space);
    if (space == 6) {
        printf("-----------------------------\n");
    }
}

// Print only the nodes that have been visited
void printVisitedTree(Node* root, int space) {
    if (!root || !root->visited) return;
    space += 6;
    printVisitedTree(root->right, space);
    printf("\n");
    for (int i = 6; i < space; i++)
        printf(" ");
    printf("[%d(%c)]\n", root->key, root->color == RED ? 'R' : 'B');
    printVisitedTree(root->left, space);
    if (space == 6) {
        printf("-----------------------------\n");
    }
}

void printLevelOrderWithQueue(Node* root) {
    if (!root) { printf("[empty tree]\n"); return; }
    Queue q = {NULL, NULL};
    enqueue(&q, root);
    int iter = 1;
    printf("BFS Traversal with Queue States\n\n");
    while (!isEmpty(&q)) {
        printf("Iteration %d\n", iter++);
        printf("Action : VISIT front node\n");
        printf("Queue before action:\n");
        printQueue(&q);
        Node* n = dequeue_q(&q);
        n->visited = 1;
        if (n->left) enqueue(&q, n->left);
        if (n->right) enqueue(&q, n->right);
        printf("Final condition:\n");
        printQueue(&q);
        if (!isEmpty(&q))
            printf("Next action : VISIT front node\n");
        else
            printf("Next action : [none]\n");
        printf("Tree Visualization:\n");
        printVisitedTree(root, 0);
        printf("-----------------------------\n");
    }
}

int read_input(const char* filename, int arr[]) {
    FILE* f = fopen(filename, "r");
    if (!f) return 0;
    char line[MAX_LINE];
    if (!fgets(line, MAX_LINE, f)) { fclose(f); return 0; }
    fclose(f);
    char* p = strchr(line, '[');
    if (!p) return 0;
    p++;
    int n = 0;
    while (*p && *p != ']') {
        while (*p == ' ' || *p == ',') p++;
        if (*p == ']') break;
        arr[n++] = strtol(p, &p, 10);
    }
    return n;
}

int main(void) {
    int values[MAX_INPUT];
    int n = read_input("input.json", values);
    if (n <= 0) {
        printf("Failed to read input.json\n");
        return 1;
    }
    Node* root = NULL;
    printf("Red-Black Tree Simulation (input: ");
    for (int i = 0; i < n; ++i) printf("%d%s", values[i], i==n-1?"":", ");
    printf(")\n\n");
    for (int i = 0; i < n; ++i) {
        printf("Insert %d\n", values[i]);
        rbInsert(&root, values[i]);
    }
    printf("\n");
    printLevelOrderWithQueue(root);
    return 0;
}
