//
// Created by Busiu on 06.03.2020.
//

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int partner_rank = (world_rank + 1) % 2;

    if (world_size != 2) {
        printf("World size must be equal 2!\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int buffer_sizes[9] = {
            100,
            100,
            1000,
            10000,
            100000,
            1000000,
            10000000,
            100000000,
            1000000000
    };

    int no_sendings = 50;
    double Mb_in_byte = 125000.0;

    if (world_rank == 0) {
        MPI_Barrier(MPI_COMM_WORLD);
        for (int i = 0; i < 9; i++) {
            char* buffer = malloc(buffer_sizes[i]);
            double total_time = 0;

            for (int j = 0; j < no_sendings; j++) {
                double start = MPI_Wtime();
                MPI_Send(buffer, buffer_sizes[i], MPI_CHAR, partner_rank, 0, MPI_COMM_WORLD);
                double end = MPI_Wtime();
                total_time += end - start;
            }

            free(buffer);
            printf("Buffer nr: %d. Bandwidth is %lf Mb/s.\n", i, (buffer_sizes[i] / Mb_in_byte) / total_time * no_sendings);
        }
    } else {
        MPI_Barrier(MPI_COMM_WORLD);
        for (int i = 0; i < 9; i++) {
            char* buffer = malloc(buffer_sizes[i]);

            for (int j = 0; j < no_sendings; j++) {
                MPI_Recv(buffer, buffer_sizes[i], MPI_CHAR, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }

            free(buffer);
        }
    }
    MPI_Finalize();

    return 0;
}
