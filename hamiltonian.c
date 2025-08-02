#include <stdio.h>
#define V 5

// Example graph represented as adjacency matrix
int graph[V][V] = {
    {0,1,0,1,0},
    {1,0,1,1,1},
    {0,1,0,0,1},
    {1,1,0,0,1},
    {0,1,1,1,0}
};

int path[V]; // Array to store the Hamiltonian cycle path

// Function to check if vertex v can be added at position pos in the Hamiltonian cycle
int isSafe(int v, int pos) {
    if (!graph[path[pos - 1]][v]) return 0; // Check if there is an edge
    for (int i = 0; i < pos; i++)
        if (path[i] == v) return 0; // Check if vertex is already included
    return 1; // Safe to add
}

// Recursive function to solve the Hamiltonian cycle problem
int hamiltonian(int pos) {
    if (pos == V) return graph[path[pos - 1]][path[0]]; // Check if last vertex connects to first
    for (int v = 1; v < V; v++) {
        if (isSafe(v, pos)) { // Check if vertex can be added
            path[pos] = v; // Add vertex to path
            if (hamiltonian(pos + 1)) return 1; // Recur to add rest
            path[pos] = -1; // Backtrack if adding vertex doesn't lead to solution
        }
    }
    return 0; // No vertex can be added at this position
}

int main() {
    for (int i = 0; i < V; i++) path[i] = -1; // Initialize path
    path[0] = 0; // Start from vertex 0
    if (hamiltonian(1)) {
        for (int i = 0; i < V; i++) printf("%d ", path[i]); // Print the cycle
        printf("%d\n", path[0]); // Print starting vertex to complete the cycle
    } else printf("No Hamiltonian cycle\n"); // If no cycle exists
    return 0;
}
