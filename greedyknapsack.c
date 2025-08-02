#include <stdio.h>
#include <stdlib.h>

// Function to solve the Fractional Knapsack problem using a greedy approach
// W: capacity, wt[]: weights, val[]: values, n: number of items
// Returns the maximum value that can be put in a knapsack of capacity W

int fracKnapsack(int W, int wt[], int val[], int n) {
    int idx[n];
    int ratio[n];
    for (int i = 0; i < n; i++) {
        idx[i] = i;
        ratio[i] = val[i] / wt[i];
    }
    // Sort indices based on value/weight ratio
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (ratio[idx[j]] > ratio[idx[i]]) {
                int temp = idx[i];
                idx[i] = idx[j];
                idx[j] = temp;
            }
        }
    }
    int total = 0;
    for (int i = 0; i < n && W; i++) {
        int k = idx[i];
        if (wt[k] <= W) {
            W -= wt[k];
            total += val[k];
        } else {
            total += val[k] * ((double)W / wt[k]);
            break;
        }
    }
    return total;
}

int main() {
    int wt[] = {10, 20, 30}; // Weights of items
    int val[] = {60, 100, 120}; // Values of items
    int W = 50, n = 3; // Knapsack capacity and number of items
    printf("Max value: %d", fracKnapsack(W, wt, val, n)); // Print the result
    return 0;
}
