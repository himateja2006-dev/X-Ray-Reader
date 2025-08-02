#include <stdio.h>

// Returns the maximum of two integers
int max(int a, int b) { return a > b ? a : b; }

// Function to solve 0/1 Knapsack problem using Dynamic Programming
// W: capacity, wt[]: weights, val[]: values, n: number of items
int knapsack(int W, int wt[], int val[], int n) {
    int dp[n + 1][W + 1]; // DP table: dp[i][w] is max value for first i items and capacity w

    // Build table dp[][] in bottom-up manner
    for (int i = 0; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            if (i == 0 || w == 0)
                dp[i][w] = 0; // Base case: no items or capacity
            else if (wt[i - 1] <= w)
                dp[i][w] = max(val[i - 1] + dp[i - 1][w - wt[i - 1]], dp[i - 1][w]); // Include or exclude item
            else
                dp[i][w] = dp[i - 1][w]; // Cannot include item
        }
    }
    return dp[n][W]; // Return max value for n items and capacity W
}

int main() {
    int val[] = {60, 100, 120}, wt[] = {10, 20, 30}, W = 50, n = 3; // Example values, weights, capacity, and item count
    printf("Max value: %d\n", knapsack(W, wt, val, n)); // Print the result
    return 0;
}
