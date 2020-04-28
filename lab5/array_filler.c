//
// Created by Busiu on 21.04.2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

void normal_filler(int *head, int no_elements) {
    double start_timer = omp_get_wtime();
    for (int i = 0; i < no_elements; i++) {
        *head = rand() % 100;
        head++;
    }
    double end_timer = omp_get_wtime();
    printf("Normal - time taken: %lf.\n", end_timer - start_timer);
}

void guided_1_filler(int* head, int no_elements) {
    double start_timer = omp_get_wtime();
    #pragma omp parallel for schedule(guided, 1)
    for (int i = 0; i < no_elements; i++) {
        *head = rand() % 100;
        head++;
    }
    double end_timer = omp_get_wtime();
    printf("Guided 1 - time taken: %lf.\n", end_timer - start_timer);
}
void guided_3_filler(int* head, int no_elements) {
    double start_timer = omp_get_wtime();
    #pragma omp parallel for schedule(guided, 3)
    for (int i = 0; i < no_elements; i++) {
        *head = rand() % 100;
        head++;
    }
    double end_timer = omp_get_wtime();
    printf("Guided 3 - time taken: %lf.\n", end_timer - start_timer);
}
void guided_5_filler(int* head, int no_elements) {
    double start_timer = omp_get_wtime();
    #pragma omp parallel for schedule(guided, 5)
    for (int i = 0; i < no_elements; i++) {
        *head = rand() % 100;
        head++;
    }
    double end_timer = omp_get_wtime();
    printf("Guided 5 - time taken: %lf.\n", end_timer - start_timer);
}
void guided_25_filler(int* head, int no_elements) {
    double start_timer = omp_get_wtime();
    #pragma omp parallel for schedule(guided, 25)
    for (int i = 0; i < no_elements; i++) {
        *head = rand() % 100;
        head++;
    }
    double end_timer = omp_get_wtime();
    printf("Guided 25 - time taken: %lf.\n", end_timer - start_timer);
}
void guided_125_filler(int* head, int no_elements) {
    double start_timer = omp_get_wtime();
    #pragma omp parallel for schedule(guided, 1250000)
    for (int i = 0; i < no_elements; i++) {
        *head = rand() % 100;
        head++;
    }
    double end_timer = omp_get_wtime();
    printf("Guided 125 - time taken: %lf.\n", end_timer - start_timer);
}

void static_1_filler(int* head, int no_elements) {
    double start_timer = omp_get_wtime();
    #pragma omp parallel for schedule(static, 1)
    for (int i = 0; i < no_elements; i++) {
        *head = rand() % 100;
        head++;
    }
    double end_timer = omp_get_wtime();
    printf("Static 1 - time taken: %lf.\n", end_timer - start_timer);
}
void static_3_filler(int* head, int no_elements) {
    double start_timer = omp_get_wtime();
    #pragma omp parallel for schedule(static, 3)
    for (int i = 0; i < no_elements; i++) {
        *head = rand() % 100;
        head++;
    }
    double end_timer = omp_get_wtime();
    printf("Static 3 - time taken: %lf.\n", end_timer - start_timer);
}
void static_5_filler(int* head, int no_elements) {
    double start_timer = omp_get_wtime();
    #pragma omp parallel for schedule(static, 5)
    for (int i = 0; i < no_elements; i++) {
        *head = rand() % 100;
        head++;
    }
    double end_timer = omp_get_wtime();
    printf("Static 5 - time taken: %lf.\n", end_timer - start_timer);
}
void static_25_filler(int* head, int no_elements) {
    double start_timer = omp_get_wtime();
    #pragma omp parallel for schedule(static, 25)
    for (int i = 0; i < no_elements; i++) {
        *head = rand() % 100;
        head++;
    }
    double end_timer = omp_get_wtime();
    printf("Static 25 - time taken: %lf.\n", end_timer - start_timer);
}
void static_125_filler(int* head, int no_elements) {
    double start_timer = omp_get_wtime();
    #pragma omp parallel for schedule(static, 125)
    for (int i = 0; i < no_elements; i++) {
        *head = rand() % 100;
        head++;
    }
    double end_timer = omp_get_wtime();
    printf("Static 125 - time taken: %lf.\n", end_timer - start_timer);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Number of arguments must be equal 1!\n");
        return -1;
    }

    int no_elements = atoi(argv[1]);
    int* int_array = malloc(no_elements * sizeof(int));
    srand(time(NULL));

    omp_set_num_threads(20);

    normal_filler(int_array, no_elements);

    guided_1_filler(int_array, no_elements);
    guided_3_filler(int_array, no_elements);
    guided_5_filler(int_array, no_elements);
    guided_25_filler(int_array, no_elements);
    guided_125_filler(int_array, no_elements);

    static_1_filler(int_array, no_elements);
    static_3_filler(int_array, no_elements);
    static_5_filler(int_array, no_elements);
    static_25_filler(int_array, no_elements);
    static_125_filler(int_array, no_elements);

    printf("%i\n", omp_get_max_threads());

    free(int_array);

    return 1;
}

