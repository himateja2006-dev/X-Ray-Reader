#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int x[30], count = 0; // x[] stores the column positions of queens, count is the number of solutions

// Function to check if a queen can be placed at row k and column i
int place(int k, int i) {
    int j;
    for (j = 1; j < k; j++) {
        // Check if another queen is in the same column or diagonal
        if ((x[j] == i) || (abs(x[j] - i) == abs(j - k)))
            return 0;
    }
    return 1; // Safe to place queen
}

// Function to print a solution
void print_sol(int n) {
    int i, j;
    count++;
    printf("\n\nSolution#%d:\n", count);
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= n; j++) {
            if (x[i] == j)
                printf("Q\t"); // Print queen
            else
                printf("*\t"); // Print empty cell
        }
        printf("\n");
    }
}

// Recursive function to solve the N-Queens problem
void queen(int k, int n) {
    int i;
    for (i = 1; i <= n; i++) {
        if (place(k, i)) { // Check if queen can be placed
            x[k] = i; // Place queen
            if (k == n)
                print_sol(n); // Print solution if all queens are placed
            else
                queen(k + 1, n); // Recur to place next queen
        }
    }
}

int main() {
    int n;
    printf("Enter the number of Queens\n");
    scanf("%d", &n);
    queen(1, n); // Start placing queens from row 1
    printf("\nTotal solutions=%d", count);
    return 0;
}