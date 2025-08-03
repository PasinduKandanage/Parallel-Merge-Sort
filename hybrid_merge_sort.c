
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <mpi.h>

#define ARRAY_SIZE 100000000
#define NUM_THREADS 8
#define CUTOFF 100000

void merge(int *arr, int left, int mid, int right);
void parallel_merge_sort(int *arr, int left, int right);
void merge_sort(int *arr, int left, int right);



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


void merge_sort(int *arr, int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}


void parallel_merge_sort(int *arr, int left, int right) {
    if (right - left <= CUTOFF) {
        merge_sort(arr, left, right);
        return;
    }
    if (left < right) {
        int mid = (left + right) / 2;
        #pragma omp task shared(arr)
        parallel_merge_sort(arr, left, mid);
        #pragma omp task shared(arr)
        parallel_merge_sort(arr, mid + 1, right);
        #pragma omp taskwait
        merge(arr, left, mid, right);
    }
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

    int *arr = NULL, *local = NULL;
    int chunk = ARRAY_SIZE / size;

    if (rank == 0) {
        arr = malloc(ARRAY_SIZE * sizeof(int));
        srand(42);
        for (int i = 0; i < ARRAY_SIZE; i++)
            arr[i] = rand() % 1000000;
    }

    
    local = malloc(chunk * sizeof(int));
    double start = MPI_Wtime();

    MPI_Scatter(arr, chunk, MPI_INT, local, chunk, MPI_INT, 0, MPI_COMM_WORLD);




    omp_set_num_threads(NUM_THREADS);
    #pragma omp parallel
    {
        #pragma omp single
        parallel_merge_sort(local, 0, chunk - 1);
    }

    int *gathered = NULL;
    if (rank == 0)
        gathered = malloc(ARRAY_SIZE * sizeof(int));
    MPI_Gather(local, chunk, MPI_INT, gathered, chunk, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int step = chunk; step < ARRAY_SIZE; step *= 2) {
            for (int i = 0; i < ARRAY_SIZE; i += 2 * step) {
                int mid = i + step - 1;
                int right = i + 2 * step - 1;
                if (right >= ARRAY_SIZE) right = ARRAY_SIZE - 1;
                if (mid < right)
                    merge(gathered, i, mid, right);
            }
        }
        double end = MPI_Wtime();
        printf("Hybrid Merge Sort Time: %.6f seconds\n", end - start);
        printf("Sorted: %s\n", is_sorted(gathered, ARRAY_SIZE) ? "Yes" : "No");
        free(gathered);
    }

    free(local);
    if (rank == 0) free(arr);
    MPI_Finalize();
    return 0;
}
