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

    // Definir o tipo derivado para SensorData
    MPI_Datatype sensor_type;
    int blocklens[2] = {1, 3}; // 1 inteiro + 3 floats
    MPI_Aint displacements[2];
    MPI_Datatype types[2] = {MPI_INT, MPI_FLOAT};

    displacements[0] = 0;
    displacements[1] = sizeof(int);

    MPI_Type_create_struct(2, blocklens, displacements, types, &sensor_type);
    MPI_Type_commit(&sensor_type);

    SensorData sensor;
    sensor.id = rank;
    sensor.values[0] = rank + 1.5;
    sensor.values[1] = rank + 2.5;
    sensor.values[2] = rank + 3.5;

    // Buffer de empacotamento
    char packed_data[1024];
    int position = 0;

    // Processo 0 empacota os dados e envia
    if (rank == 0) {
        MPI_Pack(&sensor, 1, sensor_type, packed_data, sizeof(packed_data), &position, MPI_COMM_WORLD);
        MPI_Send(packed_data, position, MPI_BYTE, 1, 0, MPI_COMM_WORLD);
        printf("Processo 0 enviou dados empacotados.\n");
    }

    // Processo 1 recebe os dados e desempacota
    if (rank == 1) {
        MPI_Recv(packed_data, sizeof(packed_data), MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        position = 0; // Reset position
        MPI_Unpack(packed_data, sizeof(packed_data), &position, &sensor, 1, sensor_type, MPI_COMM_WORLD);
        printf("Processo 1 recebeu e desempacotou: id = %d, values = (%.1f, %.1f, %.1f)\n",
               sensor.id, sensor.values[0], sensor.values[1], sensor.values[2]);
    }

    MPI_Type_free(&sensor_type);
    MPI_Finalize();
    return 0;
}
