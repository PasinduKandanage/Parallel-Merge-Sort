# Parallel-Merge-Sort
High-performance Computing - Parallel Merge Sort using Serial, OpenMP, MPI, and Hybrid (MPI + OpenMP) Implementations in C.

# Parallel Merge Sort (HPC)

This project implements the Merge Sort algorithm in C using four different approaches:

- **Serial implementation**
- **OpenMP-based parallel implementation**
- **MPI-based distributed memory implementation**
- **Hybrid implementation using both MPI and OpenMP**

Designed for high-performance computing environments, the code targets large data sets with `ARRAY_SIZE = 100,000,000`.

## 🛠 Technologies Used
- C Language
- OpenMP (shared-memory parallelism)
- MPI (distributed-memory parallelism)
- Hybrid (MPI + OpenMP)
- WSL2 on Ubuntu (tested)


## 📁 Folder Structure

.
├── serial_merge_sort.c      # Serial implementation
├── openmp_merge_sort.c      # OpenMP implementation
├── mpi_merge_sort.c         # MPI implementation
├── hybrid_merge_sort.c      # MPI + OpenMP hybrid implementation
└── README.md

## Compilation Commands 

 - gcc - o serial_m erge_sort serial_ merge_sort.c
 - gcc - fopenmp - o openmp_merge_sort openmp_merge_sort.c
 - mpicc - o mpi_merge_sort mpi_merge_sort.c
 - mpicc - fopenmp - o hybrid_merge_sort hybrid_merge_sort.c

## Run Commands 

- . / serial_merge_sort
- . / openmp_merge_sort
- mpirun - np 4 . / mpi_merge_sort
- mpirun - np 4 . / hybrid_merge_sort






