
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ARRAY_SIZE 100000000

void merge_sort(int *arr, int left, int right);
void merge(int *arr, int left, int mid, int right);

void merge_sort(int *arr, int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void merge(int *arr, int left, int mid, int right) {
    int *temp = malloc((right - left + 1) * sizeof(int));
    int i = left, j = mid + 1, k = 0;
    while (i <= mid && j <= right)
        temp[k++] = (arr[i] < arr[j]) ? arr[i++] : arr[j++];
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    for (i = left, k = 0; i <= right; i++, k++)
        arr[i] = temp[k];
    free(temp);
}

int is_sorted(int *arr, int n) {
    for (int i = 0; i < n - 1; i++)
        if (arr[i] > arr[i + 1]) return 0;
    return 1;
}

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = ARRAY_SIZE, chunk = n / size;
    int *arr = NULL, *local_arr = malloc(chunk * sizeof(int));

    if (rank == 0) {
        arr = malloc(n * sizeof(int));
        srand(42);
        for (int i = 0; i < n; i++)
            arr[i] = rand() % 1000000;
    }

    double start = MPI_Wtime();

    MPI_Scatter(arr, chunk, MPI_INT, local_arr, chunk, MPI_INT, 0, MPI_COMM_WORLD);
    merge_sort(local_arr, 0, chunk - 1);

    int *gathered = NULL;
    if (rank == 0)
        gathered = malloc(n * sizeof(int));
    MPI_Gather(local_arr, chunk, MPI_INT, gathered, chunk, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int step = chunk; step < n; step *= 2) {
            for (int i = 0; i < n; i += 2 * step) {
                int mid = i + step - 1;
                int right = i + 2 * step - 1;
                if (right >= n) right = n - 1;
                if (mid < right)
                    merge(gathered, i, mid, right);
            }
        }
        double end = MPI_Wtime();
        printf("MPI Merge Sort Time: %.6f seconds\n", end - start);
        printf("Sorted: %s\n", is_sorted(gathered, n) ? "Yes" : "No");
        free(gathered);
    }

    free(local_arr);
    if (rank == 0) free(arr);
    MPI_Finalize();
    return 0;
}
