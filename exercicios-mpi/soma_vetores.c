#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
    srand(time(NULL));

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 10;
    int n_per_proc = n / size;

    int A[n];
    int B[n];
    int C[n];
    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            A[i] = rand() % 100;
            B[i] = rand() % 100;
        }

        printf("A: ");
        for (int i = 0; i < n; i++) {
            printf("%d ", A[i]);
        }
        printf("\n");
        printf("B: ");
        for (int i = 0; i < n; i++) {
            printf("%d ", B[i]);
        }
    }

    int my_values_a[n_per_proc];
    int my_values_b[n_per_proc];
    int my_results[n_per_proc];

    MPI_Scatter( A , n_per_proc , MPI_INT , my_values_a , n_per_proc , MPI_INT , 0 , MPI_COMM_WORLD);
    MPI_Scatter( B , n_per_proc , MPI_INT , my_values_b , n_per_proc , MPI_INT , 0 , MPI_COMM_WORLD);
    
    for (int i = 0; i < n_per_proc; i++) {
        my_results[i] = my_values_a[i] + my_values_b[i];
    }

    MPI_Gather( my_results , 2 , MPI_INT , C , 2 , MPI_INT , 0 , MPI_COMM_WORLD);

    if (rank == 0) {
        printf("\nC: ");
        for (int i = 0; i < n; i++) {
            printf("%d ", C[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
}
