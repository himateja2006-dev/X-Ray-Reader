#include <stdio.h>
#define V 4
#define INF 9999

int dp[1 << V][V]; // DP table: dp[mask][pos] stores min cost to visit set 'mask' ending at 'pos'
int dist[V][V] = {
    {0, 10, 15, 20},
    {10, 0, 35, 25},
    {15, 35, 0, 30},
    {20, 25, 30, 0}
};

// Recursive function to solve TSP using DP and bitmasking
// mask: set of visited cities, pos: current city
int tsp(int mask, int pos) {
    if (mask == (1 << V) - 1) return dist[pos][0]; // All cities visited, return to start

    if (dp[mask][pos] != -1) return dp[mask][pos]; // Return memoized result

    int ans = INF; // Initialize answer to infinity
    for (int city = 0; city < V; city++) {
        if (!(mask & (1 << city))) // If city not visited yet
            ans = (dist[pos][city] + tsp(mask | (1 << city), city)) < ans ?
                  (dist[pos][city] + tsp(mask | (1 << city), city)) : ans; // Try visiting city
    }
    return dp[mask][pos] = ans; // Memoize and return answer
}

int main() {
    // Initialize DP table with -1 (uncomputed)
    for (int i = 0; i < (1 << V); i++)
        for (int j = 0; j < V; j++)
            dp[i][j] = -1;

    printf("Minimum cost: %d\n", tsp(1, 0)); // Start from city 0 with only city 0 visited
    return 0;
}
