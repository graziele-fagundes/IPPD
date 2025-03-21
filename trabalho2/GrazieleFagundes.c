// Graziele Fagundes Martins - 21201339
// mpicc -o GrazieleFagundes GrazieleFagundes.c -lm
// mpirun -np 4 ./GrazieleFagundes

// Quando um nó encontra a senha, ele avisa os outros nós e todos param a execução por meio de um MPI_Allreduce.
// Utilizando a variável global_found, que é 1 se a senha foi encontrada por algum nó e 0 caso contrário.

// Senhas utilizadas: ABCD, ABCDE, ABCDEF
// Tamanho da Senha - Tempo(s) - Nós
// 4 - 0.001107 - 2
// 5 - 0.013279 - 2
// 6 - 0.310016 - 2

// 4 - 0.001407 - 3
// 5 - 0.021683 - 3
// 6 - 0.485596 - 3
 
// 4 - 0.003062 - 4
// 5 - 0.033185 - 4
// 6 - 0.718209 - 4

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>

#define CHARSET "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define CHARSET_SIZE 26

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double start, end;
    int tamanho;
    char *senhaCorreta;
    if (rank == 0)
    {
        printf("Digite o tamanho da senha: ");
        scanf("%d", &tamanho);
        senhaCorreta = (char *)malloc(tamanho + 1);
        printf("Digite a senha de %d letras maiusculas: ", tamanho);
        scanf("%s", senhaCorreta);

        start = MPI_Wtime();
    }

    MPI_Bcast(&tamanho, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(senhaCorreta, tamanho + 1, MPI_CHAR, 0, MPI_COMM_WORLD);

    // Cada processo pega um subconjunto das combinações baseado no primeiro caractere
    int chars_per_proc = CHARSET_SIZE / size;
    int extra = CHARSET_SIZE % size;
    int start_index, end_index;

    if (rank < extra) {
        start_index = rank * (chars_per_proc + 1);
        end_index = start_index + chars_per_proc;
    } else {
        start_index = rank * chars_per_proc + extra;
        end_index = start_index + chars_per_proc - 1;
    }

    printf("Processo %d: %c-%c\n", rank, CHARSET[start_index], CHARSET[end_index]);

    // Inicializa tentativa e vetor de índices
    char tentativa[tamanho + 1];
    tentativa[tamanho] = '\0';

    int indices[tamanho];

    // Variáveis para controle de execução
    int password_found = 0;
    int global_found = 0;

    // Zera os índices e define o primeiro caractere dentro do intervalo do processo
    for (int i = 0; i < tamanho; i++) {
        indices[i] = 0;
    }
    indices[0] = start_index;

    // Enquanto o primeiro caractere está dentro do intervalo do processo
    while (indices[0] <= end_index && !password_found) {
        // Constrói a tentativa com base nos índices
        for (int i = 0; i < tamanho; i++) {
            tentativa[i] = CHARSET[indices[i]];
        }

        if (strcmp(tentativa, senhaCorreta) == 0) {
            password_found = 1;
            
            // Todos os processos trocam a informação se alguém achou
            MPI_Allreduce(&password_found, &global_found, 1, MPI_INT, MPI_LOR, MPI_COMM_WORLD);

            break;
        }

        // Todos os processos trocam a informação se alguém achou
        MPI_Allreduce(&password_found, &global_found, 1, MPI_INT, MPI_LOR, MPI_COMM_WORLD);

        // Alguém achou a senha, então parar
        if (global_found) {
            break;  
        }
    
        // Incrementa os índices (como somar 1 em base 26)
        int pos = tamanho - 1;
        indices[pos]++;
        while (pos > 0 && indices[pos] >= CHARSET_SIZE) {
            indices[pos] = 0;
            pos--;
            indices[pos]++;
        }
    }


    if (password_found)
    {
        end = MPI_Wtime();
        printf("Processo %d: Senha encontrada: %s\n", rank, tentativa);
    }
    else
    {
        end = MPI_Wtime();
        printf("Processo %d: Senha nao encontrada\n", rank);
    }

    if (rank == 0)
    {
        printf("Tempo total: %f\n", end - start);
    }
    MPI_Finalize();
    return 0;
}