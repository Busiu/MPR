//
// Created by Busiu on 23.03.2020.
//

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    long long no_points = atoll(argv[1]);

    MPI_Barrier(MPI_COMM_WORLD);

    double start = MPI_Wtime();

    srand(time(NULL) * world_rank);
    long long result = 0;
    for (long long i = 0; i < no_points / world_size; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if(x*x + y*y <= 1)
            result++;
    }

    long long points_inside;
    MPI_Reduce(&result, &points_inside, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    double end = MPI_Wtime();

    if (world_rank == 0) {
        double PI = 4.0 * points_inside / (no_points / world_size * world_size);
        printf("%lf\n", end - start);
    }

    MPI_Finalize();

    return 0;
}
