// mpicc -o ola_mpi ola_mpi.c -lm
// mpiexec -np 4 ./ola_mpi
// mpirun -np 4 ./ola_mpi

#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);               // Inicializa MPI

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // ID do processo (rank)
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Total de processos

    printf("Olá do processo %d de %d\n", rank, size);

    MPI_Finalize();                       // Finaliza MPI
    return 0;
}
