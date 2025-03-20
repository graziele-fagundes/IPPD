#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int data = rank;
    int result = 0;

    // Realiza a soma de todos os valores
    //          send_data, recv_data, count, datatype,    op,   root,   comm
    MPI_Reduce(&data,       &result,   1,    MPI_INT,   MPI_SUM, 0,   MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Processo 0 recebeu o resultado da soma: %d\n", result);
    }

    MPI_Finalize();
    return 0;
}
