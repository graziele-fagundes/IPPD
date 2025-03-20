#include <mpi.h>
#include <stdio.h>

typedef struct {
    int id;
    float values[3];
} SensorData;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Criação do tipo derivado
    MPI_Datatype sensor_type;
    int blocklens[2] = {1, 3};                      // 1 inteiro + 3 floats
    MPI_Aint displacements[2];                      // Deslocamentos
    MPI_Datatype types[2] = {MPI_INT, MPI_FLOAT};   // Tipos dos campos

    // Deslocamentos
    displacements[0] = 0;               // 'id' começa no início
    displacements[1] = sizeof(int);     // 'values' começa depois de 'id'

    // Cria o tipo derivado
    MPI_Type_create_struct(2, blocklens, displacements, types, &sensor_type);
    MPI_Type_commit(&sensor_type); // Finaliza a definição do tipo derivado

    SensorData sensor_data;
    sensor_data.id = rank;
    sensor_data.values[0] = rank + 1.5;
    sensor_data.values[1] = rank + 2.5;
    sensor_data.values[2] = rank + 3.5;


    if (rank == 0) {
        MPI_Send(&sensor_data, 1, sensor_type, 1, 0, MPI_COMM_WORLD);
        printf("Processo 0 enviou dados para o processo 1\n");
    }
    else if (rank == 1) {
        SensorData received_data;
        MPI_Recv(&received_data, 1, sensor_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Processo 1 recebeu dados do processo 0\n");
       
        printf("ID: %d\n", received_data.id);
        printf("Values: %f %f %f\n", received_data.values[0], received_data.values[1], received_data.values[2]);
    }

    MPI_Type_free(&sensor_type); // Libera o tipo derivado
    MPI_Finalize();
    return 0;
}
