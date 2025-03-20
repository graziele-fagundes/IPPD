#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Inicializar valores
    int values[size];
    if (rank == 0) {
        for (int i = 0; i < size; i++) {
            values[i] = i;
        }
    }
    if (rank == 0) {
        printf("Valores iniciais: ");
        for (int i = 0; i < size; i++) {
            printf("%d ", values[i]);
        }
        printf("\n");
    }

    // Enviar para todos os processos
    int recv_value;
    MPI_Scatter( values , 1 , MPI_INT , &recv_value , 1 , MPI_INT , 0 , MPI_COMM_WORLD );

    printf("Rank %d: valor recebido: %d\n", rank, recv_value);
    int new_value = recv_value * rank;
    printf("Rank %d: valor calculado: %d\n", rank, new_value);


    // Receber de todos os processos
    int result_values[size];
    MPI_Gather( &new_value , 1, MPI_INT , result_values , 1 , MPI_INT , 0 , MPI_COMM_WORLD);
    if (rank == 0) {
        printf("Processo 0 recebeu os dados: ");
        for (int i = 0; i < size; i++) {
            printf("%d ", result_values[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
