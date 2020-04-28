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

    if (world_rank == 0) {
        MPI_Barrier(MPI_COMM_WORLD);
        for (int i = 0; i < 9; i++) {
            char* buffer = malloc(buffer_sizes[i]);
            int attach_size = 10000; //buffer_sizes[i] + MPI_BSEND_OVERHEAD;
            MPI_Buffer_attach(buffer, attach_size);
            double start = MPI_Wtime();

            MPI_Bsend(buffer, 100, MPI_CHAR, partner_rank, 0, MPI_COMM_WORLD);
            //MPI_Recv(buffer, 10, MPI_CHAR, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            double end = MPI_Wtime();
            //MPI_Buffer_detach(&buffer, &attach_size);
            free(buffer);
            printf("Buffer nr: %d. It tooks %lf seconds.\n", i, (end - start) / 2);
        }
    } else {
        MPI_Barrier(MPI_COMM_WORLD);
        for (int i = 0; i < 9; i++) {
            char* buffer = malloc(buffer_sizes[i]);
            int attach_size = 10000; //buffer_sizes[i] + MPI_BSEND_OVERHEAD;
            //MPI_Buffer_attach(buffer, attach_size);

            MPI_Recv(buffer, 100, MPI_CHAR, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //MPI_Bsend(buffer, 10, MPI_CHAR, partner_rank, 0, MPI_COMM_WORLD);

            //MPI_Buffer_detach(&buffer, &attach_size);
            free(buffer);
        }
    }
    MPI_Finalize();

    return 0;
}
