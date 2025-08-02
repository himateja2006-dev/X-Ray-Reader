#include <stdio.h>
#include <stdlib.h>

#define V 5
#define E 7

// Arrays to represent the edges: u[] and v[] for vertices, w[] for weights
int u[E] = {0, 0, 0, 1, 2, 1, 3};
int v[E] = {1, 2, 3, 3, 3, 2, 4};
int w[E] = {10, 6, 5, 15, 4, 25, 2};

int parent[V]; // Array to store parent of each vertex for union-find

// Find function for union-find (with path compression)
int find(int i) {
    if (parent[i] == i) return i;
    return parent[i] = find(parent[i]);
}

// Union function for union-find
void unionSet(int a, int b) {
    parent[find(a)] = find(b);
}

// Swap function to help with sorting edges by weight
void swap(int i, int j) {
    int temp;
    temp = u[i]; u[i] = u[j]; u[j] = temp;
    temp = v[i]; v[i] = v[j]; v[j] = temp;
    temp = w[i]; w[i] = w[j]; w[j] = temp;
}

// Simple bubble sort to sort edges by weight
void sortEdges() {
    for (int i = 0; i < E - 1; i++) {
        for (int j = 0; j < E - i - 1; j++) {
            if (w[j] > w[j + 1]) {
                swap(j, j + 1);
            }
        }
    }
}

// Function to perform Kruskal's algorithm and print the MST
void kruskal() {
    sortEdges(); // Sort edges by weight
    for (int i = 0; i < V; i++) parent[i] = i; // Initialize each vertex as its own parent
    printf("Edges in MST:\n");
    for (int i = 0, count = 0; i < E && count < V - 1; i++) {
        if (find(u[i]) != find(v[i])) { // If including this edge doesn't form a cycle
            printf("%d - %d: %d\n", u[i], v[i], w[i]); // Print the edge
            unionSet(u[i], v[i]); // Union the sets
            count++; // Increment the count of edges in MST
        }
    }
}

int main() {
    kruskal(); // Run Kruskal's algorithm
    return 0;
}
