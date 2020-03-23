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

    if (world_size <= 1) {
        printf("World size must be greater than 1!\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int *main_tab = malloc(world_size * 3 * sizeof(int));
    int *sub_tab = malloc(3 * sizeof(int));

    MPI_Barrier(MPI_COMM_WORLD);
    if (world_rank == 0) {
        for (int i = 0; i < world_size * 3; i++) {
            main_tab[i] = 1;
        }
    }

    MPI_Scatter(main_tab, 3, MPI_INT, sub_tab, 3, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = 0; i < 3; i++) {
        printf("%d : %d\n", world_rank, sub_tab[i]);
        sub_tab[i] *= world_rank;
    }
    MPI_Gather(sub_tab, 3, MPI_INT, main_tab, 3, MPI_INT, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        for (int i = 0; i < world_size * 3; i++) {
            printf("%d\n", main_tab[i]);
        }
    }

    free(sub_tab);
    free(main_tab);

    MPI_Finalize();

    return 0;
}
