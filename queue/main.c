#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITEMS 100
#define MAX_LINE 1024

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct {
    Node* front;
    Node* rear;
} Queue;

void enqueue(Queue* q, int value) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = value;
    node->next = NULL;
    if (q->rear) q->rear->next = node;
    q->rear = node;
    if (!q->front) q->front = node;
}

int dequeue(Queue* q, int* out) {
    if (!q->front) return 0;
    Node* tmp = q->front;
    *out = tmp->data;
    q->front = tmp->next;
    if (!q->front) q->rear = NULL;
    free(tmp);
    return 1;
}

void printQueue(Queue* q) {
    Node* cur = q->front;
    printf("Front ");
    if (!cur) {
        printf("[empty]\n");
        return;
    }
    while (cur) {
        printf("[ %d ]", cur->data);
        if (cur->next) printf(" -> ");
        cur = cur->next;
    }
    printf(" Rear\n");
}

int read_input(const char* filename, int values[]) {
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
        values[n++] = strtol(p, &p, 10);
    }
    return n;
}

int main(void) {
    int values[MAX_ITEMS];
    int count = read_input("input.json", values);
    if (count <= 0) {
        printf("Failed to read input.json\n");
        return 1;
    }

    Queue q = {NULL, NULL};
    int iter = 1;

    printf("Queue Simulation\n\n");
    for (int i = 0; i < count; ++i) {
        printf("Iteration %d\n", iter++);
        printf("Action : ENQUEUE %d\n", values[i]);
        enqueue(&q, values[i]);
        printf("Final condition:\n");
        printQueue(&q);
        if (i < count - 1)
            printf("Next action : ENQUEUE %d\n", values[i + 1]);
        else
            printf("Next action : DEQUEUE\n");
        printf("-----------------------------\n");
    }

    int val;
    while (dequeue(&q, &val)) {
        printf("Iteration %d\n", iter++);
        printf("Action : DEQUEUE -> %d\n", val);
        printf("Final condition:\n");
        printQueue(&q);
        if (q.front)
            printf("Next action : DEQUEUE\n");
        else
            printf("Next action : [none]\n");
        printf("-----------------------------\n");
    }

    return 0;
}
