#include <stdio.h>
#define V 4
#define INF 9999

// Function to implement Floyd-Warshall algorithm for all-pairs shortest paths
void floydWarshall(int g[V][V]) {
    int d[V][V]; // Matrix to store shortest distances
    // Initialize distance matrix with input graph values
    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            d[i][j] = g[i][j];

    // Update the distance matrix considering each vertex as an intermediate point
    for (int k = 0; k < V; k++)
        for (int i = 0; i < V; i++)
            for (int j = 0; j < V; j++)
                if (d[i][k] + d[k][j] < d[i][j])
                    d[i][j] = d[i][k] + d[k][j]; // Update if shorter path found

    // Print the shortest distance matrix
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++)
            printf("%4d", d[i][j]);
        printf("\n");
    }
}

int main() {
    // Example graph represented as adjacency matrix
    int g[V][V] = {
        {0, 5, INF, 10},
        {INF, 0, 3, INF},
        {INF, INF, 0, 1},
        {INF, INF, INF, 0}
    };
    floydWarshall(g); // Run Floyd-Warshall algorithm
    return 0;
}
