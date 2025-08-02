#include <stdio.h>

// Merges two sorted subarrays of a[]
// First subarray is a[l..m]
// Second subarray is a[m+1..r]
void merge(int a[], int l, int m, int r) {
    int i = 0, j = 0, k = l;
    int n1 = m - l + 1, n2 = r - m; // Sizes of the two subarrays
    int L[n1], R[n2]; // Temporary arrays to hold the subarrays
    // Copy data to temporary arrays L[] and R[]
    for (i = 0; i < n1; i++) L[i] = a[l + i];
    for (j = 0; j < n2; j++) R[j] = a[m + 1 + j];
    i = j = 0;
    // Merge the temporary arrays back into a[l..r]
    while (i < n1 && j < n2) a[k++] = (L[i] < R[j]) ? L[i++] : R[j++];
    // Copy any remaining elements of L[]
    while (i < n1) a[k++] = L[i++];
    // Copy any remaining elements of R[]
    while (j < n2) a[k++] = R[j++];
}

// Main function that sorts a[l..r] using merge sort
void mergeSort(int a[], int l, int r) {
    if (l < r) {
        int m = (l + r)/2; // Find the middle point
        mergeSort(a, l, m); // Sort first half
        mergeSort(a, m+1, r); // Sort second half
        merge(a, l, m, r); // Merge the sorted halves
    }
}

int main() {
    int a[] = {5, 3, 8, 4, 2}, n = 5; // Example array and its size
    mergeSort(a, 0, n - 1); // Call mergeSort on the entire array
    // Print the sorted array
    for (int i = 0; i < n; i++) printf("%d ", a[i]);
    return 0;
}
