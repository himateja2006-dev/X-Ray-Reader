#include <stdio.h>
#include <limits.h>

// Recursive function to solve the Traveling Salesman Problem using brute force
// g: adjacency matrix, path: current path, n: number of cities, vis: visited array
// pos: current city, count: number of cities visited, cost: current cost, start: starting city
int tsp(int g[10][10], int path[], int n, int vis[], int pos, int count, int cost, int start) {
    // If all cities are visited and there is a return path, return total cost
    if (count == n && g[pos][start]) return cost + g[pos][start];

    int minCost = INT_MAX; // Initialize minimum cost
    for (int i = 0; i < n; i++) {
        if (!vis[i] && g[pos][i]) { // If city i is not visited and there is a path
            vis[i] = 1; // Mark city as visited
            int res = tsp(g, path, n, vis, i, count + 1, cost + g[pos][i], start); // Recurse
            minCost = (res < minCost) ? res : minCost; // Update minimum cost
            vis[i] = 0; // Backtrack: unmark city
        }
    }
    return minCost; // Return the minimum cost found
}

int main() {
    // Example graph represented as adjacency matrix (4 cities)
    int g[10][10] = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };
    int vis[10] = {0}, path[10]; // Visited array and path array
    vis[0] = 1; // Start from city 0
    printf("Min cost: %d", tsp(g, path, 4, vis, 0, 1, 0, 0)); // Print the minimum cost
    return 0;
}
