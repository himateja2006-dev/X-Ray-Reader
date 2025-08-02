#include <stdio.h>
#define V 5
#define INF 9999

// Function to find the vertex with the minimum distance value not yet visited
int minDist(int dist[], int vis[]) {
    int min = INF, idx = -1;
    for (int i = 0; i < V; i++)
        if (!vis[i] && dist[i] < min)
            min = dist[i], idx = i;
    return idx;
}

// Function to implement Dijkstra's shortest path algorithm
void dijkstra(int graph[V][V], int src) {
    int dist[V], vis[V] = {0}; // dist[]: shortest distances, vis[]: visited vertices
    for (int i = 0; i < V; i++) dist[i] = INF; // Initialize all distances as infinite
    dist[src] = 0; // Distance to source is 0

    for (int i = 0; i < V - 1; i++) {
        int u = minDist(dist, vis); // Pick the minimum distance vertex not yet visited
        vis[u] = 1; // Mark the picked vertex as visited
        for (int v = 0; v < V; v++)
            if (!vis[v] && graph[u][v] && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v]; // Update distance if shorter path found
    }

    // Print the shortest distances from source to all vertices
    for (int i = 0; i < V; i++)
        printf("Distance to %d = %d\n", i, dist[i]);
}

int main() {
    // Example graph represented as adjacency matrix
    int graph[V][V] = {
        {0, 10, 0, 30, 100},
        {10, 0, 50, 0, 0},
        {0, 50, 0, 20, 10},
        {30, 0, 20, 0, 60},
        {100, 0, 10, 60, 0}
    };
    dijkstra(graph, 0); // Run Dijkstra's algorithm from vertex 0
    return 0;
}
