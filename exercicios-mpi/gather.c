// Receber de todos os processos

#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int send_data = rank;  // Cada processo envia seu rank
    int gather_data[size];

    // O processo 0 coleta os dados de todos os processos
    //           send_data, send_count, send_type,  gather_data, gather_count, gather_type, root,      comm
    MPI_Gather(&send_data,      1,       MPI_INT,   gather_data,     1,          MPI_INT,    0,   MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Processo 0 recebeu os dados: ");
        for (int i = 0; i < size; i++) {
            printf("%d ", gather_data[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
