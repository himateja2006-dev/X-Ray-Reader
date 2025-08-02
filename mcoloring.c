#include <stdio.h>
#define V 4

// Example graph represented as adjacency matrix
int graph[V][V] = {
    {0,1,1,1},
    {1,0,1,0},
    {1,1,0,1},
    {1,0,1,0}
};

int color[V]; // Array to store colors assigned to each vertex

// Function to check if it's safe to assign color c to vertex v
int isSafe(int v, int c) {
    for (int i = 0; i < V; i++)
        if (graph[v][i] && color[i] == c)
            return 0; // Adjacent vertex has the same color
    return 1; // Safe to assign color
}

// Recursive function to solve the M-Coloring problem
int mColoring(int v, int m) {
    if (v == V) return 1; // All vertices are colored
    for (int c = 1; c <= m; c++) { // Try all colors
        if (isSafe(v, c)) { // Check if color c can be assigned
            color[v] = c; // Assign color
            if (mColoring(v + 1, m)) return 1; // Recur for next vertex
            color[v] = 0; // Backtrack if coloring doesn't lead to solution
        }
    }
    return 0; // No color can be assigned to this vertex
}

int main() {
    int m = 3; // Number of colors
    if (mColoring(0, m)) {
        for (int i = 0; i < V; i++) printf("V%d: Color %d\n", i, color[i]); // Print coloring
    } else {
        printf("No solution with %d colors\n", m); // If no solution exists
    }
    return 0;
}
