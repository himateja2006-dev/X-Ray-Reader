#include <stdio.h>
#define SIZE 10

// Queue and visited array for BFS
int queue[SIZE], front = 0, rear = -1, visited[SIZE];

// Performs Breadth-First Search on the graph from the start node
void bfs(int graph[SIZE][SIZE], int n, int start) {
    visited[start] = 1; // Mark the start node as visited
    queue[++rear] = start; // Enqueue the start node
    while (front <= rear) { // While queue is not empty
        int u = queue[front++]; // Dequeue a vertex from queue
        printf("%d ", u); // Print the visited node
        for (int v = 0; v < n; v++) // Check all adjacent vertices
            if (graph[u][v] && !visited[v]) { // If there is an edge and not visited
                visited[v] = 1; // Mark as visited
                queue[++rear] = v; // Enqueue the adjacent node
            }
    }
}

int main() {
    // Example graph represented as adjacency matrix
    int graph[SIZE][SIZE] = {
        {0,1,1,0},
        {1,0,0,1},
        {1,0,0,1},
        {0,1,1,0}
    };
    bfs(graph, 4, 0); // Perform BFS starting from node 0
    return 0;
}
