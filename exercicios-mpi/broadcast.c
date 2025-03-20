// Enviar pra todos os processos

#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int data = 0;

    if (rank == 0) {
        data = 100; // O processo 0 define o valor de `data`
    }

    // Broadcast de 0 para todos os processos
    //          buffer, count, datatype, root,    comm
    MPI_Bcast(&data,      1,    MPI_INT,  0,    MPI_COMM_WORLD);

    printf("Processo %d recebeu data = %d\n", rank, data);

    MPI_Finalize();
    return 0;
}
