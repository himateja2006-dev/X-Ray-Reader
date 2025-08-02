#include <stdio.h>

// Returns the maximum of two integers
int max(int a, int b) { return a > b ? a : b; }

// Recursive function to solve 0/1 Knapsack problem using brute force
// W: remaining capacity, wt[]: weights, val[]: values, n: number of items left
int knapsack(int W, int wt[], int val[], int n) {
    if (n == 0 || W == 0) return 0; // Base case: no items or capacity left
    if (wt[n - 1] > W) return knapsack(W, wt, val, n - 1); // Skip if item too heavy
    // Return max of including or excluding current item
    return max(
        val[n - 1] + knapsack(W - wt[n - 1], wt, val, n - 1), // Include item
        knapsack(W, wt, val, n - 1) // Exclude item
    );
}

int main() {
    int val[] = {60, 100, 120}, wt[] = {10, 20, 30}, W = 50, n = 3; // Example values, weights, capacity, and item count
    printf("Max value: %d", knapsack(W, wt, val, n)); // Print the result
    return 0;
}
