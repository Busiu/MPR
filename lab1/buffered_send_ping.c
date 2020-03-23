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

    int no_sendings = 10000;

    if (world_rank == 0) {
        MPI_Barrier(MPI_COMM_WORLD);
        char* buffer = malloc(sizeof(char));
        double start = MPI_Wtime();
        for (int i = 0; i < no_sendings; i++) {
            int attach_size = 1000;
            MPI_Buffer_attach(buffer, attach_size);

            MPI_Bsend(buffer, sizeof(char), MPI_CHAR, partner_rank, 0, MPI_COMM_WORLD);
            MPI_Recv(buffer, sizeof(char), MPI_CHAR, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            MPI_Buffer_detach(&buffer, &attach_size);
        }
        double end = MPI_Wtime();
        free(buffer);
        printf("It tooks %lf.\n", (end - start) / 2);
    } else {
        MPI_Barrier(MPI_COMM_WORLD);
        char* buffer = malloc(sizeof(char));
        for (int i = 0; i < no_sendings; i++) {
            int attach_size = 1000;
            MPI_Buffer_attach(buffer, attach_size);

            MPI_Recv(buffer, sizeof(char), MPI_CHAR, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Bsend(buffer, sizeof(char), MPI_CHAR, partner_rank, 0, MPI_COMM_WORLD);

            MPI_Buffer_detach(&buffer, &attach_size);
        }
        free(buffer);
    }
    MPI_Finalize();

    return 0;
}