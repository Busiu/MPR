//
// Created by Busiu on 16.03.2020.
//

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {

    long long no_points = atoll(argv[1]);
    long long points_inside = 0;
    srand(time(NULL));

    for(long long i = 0; i < no_points; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if(x*x + y*y <= 1)
            points_inside++;
    }

    double PI = 4.0 * points_inside / no_points;

    printf("%lf\n", PI);

    return 0;
}
