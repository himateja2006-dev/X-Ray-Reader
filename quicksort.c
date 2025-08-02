#include <stdio.h>

// Swaps two elements in the array using their indices
void swap(int a[], int i, int j) {
    int t = a[i];
    a[i] = a[j];
    a[j] = t;
}

// Partitions the array around the pivot
// Elements less than pivot go to left, greater to right
int partition(int a[], int low, int high) {
    int pivot = a[high], i = low - 1; // Choose last element as pivot
    for (int j = low; j < high; j++)
        if (a[j] < pivot) swap(a, ++i, j); // Place smaller elements to left
    swap(a, i + 1, high); // Place pivot in correct position
    return i + 1; // Return the index of pivot
}

// Main function that implements QuickSort
void quickSort(int a[], int low, int high) {
    if (low < high) {
        int pi = partition(a, low, high); // Partitioning index
        quickSort(a, low, pi - 1); // Recursively sort left subarray
        quickSort(a, pi + 1, high); // Recursively sort right subarray
    }
}

int main() {
    int a[] = {9, 4, 6, 2, 1}, n = 5; // Example array and its size
    quickSort(a, 0, n - 1); // Call quickSort on the entire array
    // Print the sorted array
    for (int i = 0; i < n; i++) printf("%d ", a[i]);
    return 0;
}
