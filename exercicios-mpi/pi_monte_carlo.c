#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define TOTAL_POINTS 1000000 // Número total de pontos a serem gerados

int main(int argc, char** argv) {
    int rank, size, i, local_count = 0, global_count;
    double x, y;
    int points_per_process;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    points_per_process = TOTAL_POINTS / size; // Divide os pontos entre os processos

    srand(time(NULL) + rank);

    for (i = 0; i < points_per_process; i++) {
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;

        if ((x * x + y * y) <= 1.0) {
            local_count++;
        }
    }

    MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double pi = 4.0 * global_count / TOTAL_POINTS;
        printf("Estimativa de Pi: %f\n", pi);
    }

    MPI_Finalize();
    return 0;
}
