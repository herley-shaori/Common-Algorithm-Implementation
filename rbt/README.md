# 🌳 Red-Black Tree Demo

This example shows how to build a red-black tree from a JSON input file and then walk through it with a breadth-first traversal. During BFS the program prints the queue at every step and displays the part of the tree that has been visited.

## Big Picture

1. **Build the tree** from numbers provided in `input.json`.
2. **Traverse with BFS**, printing the queue before and after visiting each node.
3. **Visualize** the visited portion of the tree using ASCII art.

Each step is split into smaller pieces which are explained below.

## Building the Tree

The core of the program is a classical red-black tree insertion routine:

- `newNode` creates a node with `RED` as the default color and a `visited` flag set to `0`.
- `rbInsert` places the node in the binary-search-tree position and calls `insertFixup`.
- `insertFixup` uses rotations and color flips to restore the red-black properties.

These operations keep the tree balanced so that later traversals are efficient.

## BFS Traversal

The function `printLevelOrderWithQueue` performs the traversal and shows the queue at every iteration.

```c
Queue q = {NULL, NULL};
enqueue(&q, root);
int iter = 1;
while (!isEmpty(&q)) {
    printf("Iteration %d\n", iter++);
    printQueue(&q);                     // queue before visit
    Node* n = dequeue_q(&q);            // visit
    n->visited = 1;
    if (n->left)  enqueue(&q, n->left);
    if (n->right) enqueue(&q, n->right);
    printQueue(&q);                     // queue after visit
    printVisitedTree(root, 0);          // show visited part
}
```

`printVisitedTree` only renders nodes whose `visited` flag is set, giving a sense of how the tree is explored layer by layer.

## Running the Demo

From the `rbt` directory run:

```bash
./run.sh
```

The script compiles `main.c` and pipes the program output to `output.txt`. Check that file to see a complete iteration log like:

```
Iteration 1
Queue: [ 2(B) ]
Final condition:
Queue: [ 1(B) ] -> [ 4(R) ]
...
```

Enjoy watching the tree grow and the queue evolve! 😄

