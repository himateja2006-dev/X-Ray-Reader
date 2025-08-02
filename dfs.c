#include <stdio.h>
#define SIZE 10

// Visited array to keep track of visited nodes
int visited[SIZE];

// Performs Depth-First Search on the graph from node u
void dfs(int graph[SIZE][SIZE], int n, int u) {
    visited[u] = 1; // Mark the current node as visited
    printf("%d ", u); // Print the visited node
    for (int v = 0; v < n; v++) // Check all adjacent vertices
        if (graph[u][v] && !visited[v]) // If there is an edge and not visited
            dfs(graph, n, v); // Recursively visit the adjacent node
}

int main() {
    // Example graph represented as adjacency matrix
    int graph[SIZE][SIZE] = {
        {0,1,1,0},
        {1,0,0,1},
        {1,0,0,1},
        {0,1,1,0}
    };
    dfs(graph, 4, 0); // Perform DFS starting from node 0
    return 0;
}
