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
        printf("  Tree rebalancing occurs! Right rotate at %d.\n", node->key);
        node = rightRotate(node);
        return node;
    }
    if (balance < -1 && key > node->right->key) {
        printf("  Tree unbalanced (Right Right) after inserting %d.\n", key);
        printf("  Tree rebalancing occurs! Left rotate at %d.\n", node->key);
        node = leftRotate(node);
        return node;
    }
    if (balance > 1 && key > node->left->key) {
        printf("  Tree unbalanced (Left Right) after inserting %d.\n", key);
        printf("  Tree rebalancing occurs! Left rotate at %d then right rotate at %d.\n", node->left->key, node->key);
        node->left = leftRotate(node->left);
        node = rightRotate(node);
        return node;
    }
    if (balance < -1 && key < node->right->key) {
        printf("  Tree unbalanced (Right Left) after inserting %d.\n", key);
        printf("  Tree rebalancing occurs! Right rotate at %d then left rotate at %d.\n", node->right->key, node->key);
        node->right = rightRotate(node->right);
        node = leftRotate(node);
        return node;
    }
    return node;
}

// Tree printing structures and functions
typedef struct {
    Node *node;
    int x, y;
} NodePos;

typedef struct {
    char **canvas;
    int width;
    int height;
} Canvas;

int getTreeHeight(Node *root) {
    if (root == NULL) return 0;
    return 1 + max(getTreeHeight(root->left), getTreeHeight(root->right));
}

int getNumberWidth(int num) {
    if (num < 0) num = -num;
    if (num < 10) return 1;
    if (num < 100) return 2;
    if (num < 1000) return 3;
    return 4;
}

void fillPositions(Node *root, NodePos *positions, int *count, int x, int y, int offset) {
    if (root == NULL) return;
    
    positions[*count].node = root;
    positions[*count].x = x;
    positions[*count].y = y;
    (*count)++;
    
    // Adjust offset for tighter spacing
    int newOffset = offset > 4 ? offset / 2 : 2;
    fillPositions(root->left, positions, count, x - offset, y + 2, newOffset);
    fillPositions(root->right, positions, count, x + offset, y + 2, newOffset);
}

Canvas* createCanvas(int width, int height) {
    Canvas *canvas = (Canvas*)malloc(sizeof(Canvas));
    canvas->width = width;
    canvas->height = height;
    canvas->canvas = (char**)malloc(height * sizeof(char*));
    
    for (int i = 0; i < height; i++) {
        canvas->canvas[i] = (char*)malloc(width * sizeof(char));
        memset(canvas->canvas[i], ' ', width);
    }
    
    return canvas;
}

void freeCanvas(Canvas *canvas) {
    for (int i = 0; i < canvas->height; i++) {
        free(canvas->canvas[i]);
    }
    free(canvas->canvas);
    free(canvas);
}

void drawOnCanvas(Canvas *canvas, int x, int y, const char *str) {
    int len = strlen(str);
    for (int i = 0; i < len && x + i < canvas->width; i++) {
        if (x + i >= 0) {
            canvas->canvas[y][x + i] = str[i];
        }
    }
}

void printTree(Node *root, int unused) {
    if (root == NULL) {
        printf("(empty tree)\n");
        return;
    }
    
    int treeHeight = getTreeHeight(root);
    int canvasHeight = (treeHeight * 2) - 1;
    int canvasWidth = 120;  // Fixed width for better control
    
    Canvas *canvas = createCanvas(canvasWidth, canvasHeight);
    
    // Calculate positions
    int maxNodes = (1 << treeHeight) - 1;
    NodePos *positions = (NodePos*)malloc(maxNodes * sizeof(NodePos));
    int nodeCount = 0;
    
    // Start with tighter initial offset
    int initialOffset = treeHeight > 3 ? (1 << (treeHeight - 1)) : 4;
    fillPositions(root, positions, &nodeCount, canvasWidth / 2, 0, initialOffset);
    
    // Draw nodes and branches
    for (int i = 0; i < nodeCount; i++) {
        Node *node = positions[i].node;
        int x = positions[i].x;
        int y = positions[i].y;
        
        // Draw the number
        char numStr[10];
        sprintf(numStr, "%d", node->key);
        int numWidth = strlen(numStr);
        drawOnCanvas(canvas, x - numWidth / 2, y, numStr);
        
        // Draw branches if there are children
        if (y + 2 < canvasHeight) {
            // Find children positions
            int leftChildX = -1, rightChildX = -1;
            
            for (int j = 0; j < nodeCount; j++) {
                if (positions[j].y == y + 2) {
                    if (node->left && positions[j].node == node->left) {
                        leftChildX = positions[j].x;
                    }
                    if (node->right && positions[j].node == node->right) {
                        rightChildX = positions[j].x;
                    }
                }
            }
            
            // Draw branches with proper spacing
            if (leftChildX != -1 || rightChildX != -1) {
                int branchY = y + 1;
                
                if (leftChildX != -1 && rightChildX != -1) {
                    // Both children exist
                    int leftBranchX = x - 1;
                    int rightBranchX = x + numWidth;
                    
                    // Draw slashes between parent and children
                    drawOnCanvas(canvas, leftBranchX, branchY, "/");
                    drawOnCanvas(canvas, rightBranchX, branchY, "\\");
                    
                    // Fill spaces between branches
                    for (int j = leftBranchX + 1; j < rightBranchX; j++) {
                        canvas->canvas[branchY][j] = ' ';
                    }
                    
                } else if (leftChildX != -1) {
                    // Only left child
                    drawOnCanvas(canvas, x - 1, branchY, "/");
                } else {
                    // Only right child
                    drawOnCanvas(canvas, x + numWidth, branchY, "\\");
                }
            }
        }
    }
    
    // Print the canvas
    for (int i = 0; i < canvasHeight; i++) {
        // Find the rightmost non-space character
        int lastChar = canvasWidth - 1;
        while (lastChar >= 0 && canvas->canvas[i][lastChar] == ' ') {
            lastChar--;
        }
        
        // Find the leftmost non-space character
        int firstChar = 0;
        while (firstChar < canvasWidth && canvas->canvas[i][firstChar] == ' ') {
            firstChar++;
        }
        
        // Print the line if it has content
        if (firstChar <= lastChar) {
            // Add some left padding for centering
            printf("         ");
            for (int j = firstChar; j <= lastChar; j++) {
                printf("%c", canvas->canvas[i][j]);
            }
            printf("\n");
        }
    }
    
    printf("\n");
    
    // Clean up
    freeCanvas(canvas);
    free(positions);
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
        printf("\nCurrent tree:\n");
        printTree(root, 0);
        printf("-----------------------------\n\n");
    }
    return 0;
}