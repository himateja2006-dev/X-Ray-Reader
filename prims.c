#include <stdio.h>
#define V 5
#define INF 9999

// Function to find the vertex with the minimum key value not yet included in MST
int minKey(int key[], int mst[]) {
    int min = INF, minIndex;
    for (int i = 0; i < V; i++)
        if (!mst[i] && key[i] < min)
            min = key[i], minIndex = i;
    return minIndex;
}

// Function to construct and print MST using Prim's algorithm
void prim(int graph[V][V]) {
    int key[V], mst[V] = {0}, parent[V]; // key[]: min edge weight, mst[]: included in MST, parent[]: MST structure
    for (int i = 0; i < V; i++) key[i] = INF; // Initialize all keys as infinite
    key[0] = 0; parent[0] = -1; // Start from first vertex

    for (int count = 0; count < V - 1; count++) {
        int u = minKey(key, mst); // Pick the minimum key vertex not yet in MST
        mst[u] = 1; // Add the picked vertex to MST
        for (int v = 0; v < V; v++)
            if (graph[u][v] && !mst[v] && graph[u][v] < key[v])
                parent[v] = u, key[v] = graph[u][v]; // Update key and parent for adjacent vertices
    }

    // Print the constructed MST
    for (int i = 1; i < V; i++)
        printf("%d - %d: %d\n", parent[i], i, graph[i][parent[i]]);
}

int main() {
    // Example graph represented as adjacency matrix
    int graph[V][V] = {
        {0, 2, 0, 6, 0},
        {2, 0, 3, 8, 5},
        {0, 3, 0, 0, 7},
        {6, 8, 0, 0, 9},
        {0, 5, 7, 9, 0}
    };
    prim(graph); // Run Prim's algorithm
    return 0;
}
