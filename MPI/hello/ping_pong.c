//
// Created by Busiu on 02.03.2020.
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

    int no_messages = 10;
    int answer;
    if (world_rank == 0) {
        for (int i = 0; i < no_messages; i++) {
            MPI_Send(&i, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
            printf("Process %d. Sent %d\n", world_rank, i);
            MPI_Recv(&answer, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Process %d. Received %d\n", world_rank, answer);
        }
    } else {
        for (int i = 0; i < no_messages; i++) {
            MPI_Recv(&answer, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Process %d. Received %d\n", world_rank, answer);
            answer++;
            MPI_Send(&answer, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
            printf("Process %d. Sent %d\n", world_rank, answer);
        }
    }
    MPI_Finalize();

    return 0;
}
