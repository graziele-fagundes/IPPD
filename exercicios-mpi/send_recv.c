#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    if (rank == 0) {
        MPI_Send(&rank, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Processo 0 enviou %d para o processo 1\n", rank);
    }

    if (rank > 0 && rank < (size-1)) {
        int soma_rank_recebido;

        MPI_Recv(&soma_rank_recebido, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Processo %d recebeu %d do processo %d\n", rank, soma_rank_recebido, rank-1);
        
        int nova_soma = soma_rank_recebido + rank;
        MPI_Send(&nova_soma, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
        printf("Processo %d enviou %d para o processo %d\n", rank, nova_soma, rank+1);
    }

    if (rank == size-1) {
        int soma_rank_recebido;

        //         var               count  datatype  source  tag     comunicador     status
        MPI_Recv(&soma_rank_recebido, 1,     MPI_INT,   rank-1,     0,   MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Processo %d recebeu %d do processo %d\n", rank, soma_rank_recebido, rank-1);

        printf("Soma final: %d\n", soma_rank_recebido + rank);
    }

    MPI_Finalize();
    return 0;
}
