#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 100
#define MAX_LINE 1024

// AVL Tree Node
typedef struct Node {
    int key;
    struct Node *left, *right;
    int height;
} Node;

// Forward declaration for printTree so it can be used in insert()
void printTree(Node* root, int space);

// Utility functions
int max(int a, int b) { return (a > b) ? a : b; }

int height(Node *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

Node* newNode(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}

Node *rightRotate(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

Node *leftRotate(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

int getBalance(Node *N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

Node* insert(Node* node, int key) {
    if (node == NULL) {
        printf("  %d inserted as a new node.\n", key);
        return newNode(key);
    }
    if (key < node->key) {
        printf("  %d < %d: go left.\n", key, node->key);
        node->left = insert(node->left, key);
    } else if (key > node->key) {
        printf("  %d > %d: go right.\n", key, node->key);
        node->right = insert(node->right, key);
    } else {
        printf("  %d already exists, skipping.\n", key);
        return node;
    }
    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);
    // Print before rebalance if needed
    if (balance > 1 && key < node->left->key) {
        printf("  Tree unbalanced (Left Left) after inserting %d.\n", key);
        printf("  Before rebalance:\n");
        printTree(node, 0);
        printf("  Tree rebalancing occurs! Right rotate at %d.\n", node->key);
        node = rightRotate(node);
        printf("  After rebalance:\n");
        printTree(node, 0);
        return node;
    }
    if (balance < -1 && key > node->right->key) {
        printf("  Tree unbalanced (Right Right) after inserting %d.\n", key);
        printf("  Before rebalance:\n");
        printTree(node, 0);
        printf("  Tree rebalancing occurs! Left rotate at %d.\n", node->key);
        node = leftRotate(node);
        printf("  After rebalance:\n");
        printTree(node, 0);
        return node;
    }
    if (balance > 1 && key > node->left->key) {
        printf("  Tree unbalanced (Left Right) after inserting %d.\n", key);
        printf("  Before rebalance:\n");
        printTree(node, 0);
        printf("  Tree rebalancing occurs! Left rotate at %d then right rotate at %d.\n", node->left->key, node->key);
        node->left = leftRotate(node->left);
        node = rightRotate(node);
        printf("  After rebalance:\n");
        printTree(node, 0);
        return node;
    }
    if (balance < -1 && key < node->right->key) {
        printf("  Tree unbalanced (Right Left) after inserting %d.\n", key);
        printf("  Before rebalance:\n");
        printTree(node, 0);
        printf("  Tree rebalancing occurs! Right rotate at %d then left rotate at %d.\n", node->right->key, node->key);
        node->right = rightRotate(node->right);
        node = leftRotate(node);
        printf("  After rebalance:\n");
        printTree(node, 0);
        return node;
    }
    return node;
}

// Print tree as ASCII art
void printTree(Node* root, int space) {
    if (root == NULL)
        return;
    space += 6;
    printTree(root->right, space);
    printf("\n");
    for (int i = 6; i < space; i++)
        printf(" ");
    printf("%d\n", root->key);
    printTree(root->left, space);
    // After printing the whole tree, print a separator if at the top level
    if (space == 6) {
        printf("-----------------------------\n");
    }
}

// Read input.json (simple parser for array of ints)
int read_input(const char* filename, int* arr) {
    FILE* f = fopen(filename, "r");
    if (!f) return 0;
    char line[MAX_LINE];
    if (!fgets(line, MAX_LINE, f)) { fclose(f); return 0; }
    fclose(f);
    int n = 0;
    char* p = strchr(line, '[');
    if (!p) return 0;
    p++;
    while (*p && *p != ']') {
        while (*p == ' ' || *p == ',') p++;
        if (*p >= '0' && *p <= '9') {
            arr[n++] = strtol(p, &p, 10);
        } else {
            p++;
        }
    }
    return n;
}

int main() {
    int arr[MAX_INPUT];
    int n = read_input("input.json", arr);
    if (n == 0) {
        printf("Failed to read input.json\n");
        return 1;
    }
    Node* root = NULL;
    printf("AVL Tree Simulation (input: ");
    for (int i = 0; i < n; i++) printf("%d%s", arr[i], i==n-1?"":", ");
    printf(")\n\n");
    for (int i = 0; i < n; i++) {
        printf("Iteration %d: Insert %d\n", i+1, arr[i]);
        root = insert(root, arr[i]);
        printTree(root, 0);
        printf("\n-----------------------------\n");
    }
    return 0;
}
