//
// Created by Busiu on 09.03.2020.
//

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int ints_in_10MB = 2500000;

int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_size <= 1) {
        printf("World size must be greater than 1!\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int *main_tab = malloc(world_size * ints_in_10MB * sizeof(int));
    int *sub_tab = malloc(ints_in_10MB * sizeof(int));

    MPI_Barrier(MPI_COMM_WORLD);
    if (world_rank == 0) {
        for (int i = 0; i < world_size * ints_in_10MB; i++) {
            main_tab[i] = 1;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double start_scatter = MPI_Wtime();
    MPI_Scatter(main_tab, ints_in_10MB, MPI_INT, sub_tab, ints_in_10MB, MPI_INT, 0, MPI_COMM_WORLD);
    double end_scatter = MPI_Wtime();
    MPI_Barrier(MPI_COMM_WORLD);
    double start_gather = MPI_Wtime();
    MPI_Gather(sub_tab, ints_in_10MB, MPI_INT, main_tab, ints_in_10MB, MPI_INT, 0, MPI_COMM_WORLD);
    double end_gather = MPI_Wtime();

    printf("%d scatter : %lf\n", world_rank, end_scatter - start_scatter);
    printf("%d gather: %lf\n", world_rank, end_gather - start_gather);

    free(sub_tab);
    free(main_tab);

    MPI_Finalize();

    return 0;
}