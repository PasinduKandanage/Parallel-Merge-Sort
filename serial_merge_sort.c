
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 100000000

void merge(int *arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    free(L); free(R);
}

void serial_merge_sort(int *arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        serial_merge_sort(arr, left, mid);
        serial_merge_sort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

int is_sorted(int *arr, int n) {
    for (int i = 0; i < n - 1; i++)
        if (arr[i] > arr[i + 1]) return 0;
    return 1;
}

int main() {
    int *arr = malloc(ARRAY_SIZE * sizeof(int));
    srand(time(NULL));
    for (int i = 0; i < ARRAY_SIZE; i++)
        arr[i] = rand() % 1000000;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    serial_merge_sort(arr, 0, ARRAY_SIZE - 1);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_taken = (end.tv_sec - start.tv_sec) + 
                        (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Serial Merge Sort Time: %.6f seconds\n", time_taken);
    printf("Sorted: %s\n", is_sorted(arr, ARRAY_SIZE) ? "Yes" : "No");
    free(arr);
    return 0;
}
