//
// Created by Busiu on 28.04.2020.
//
/*
 * This is the implementation of the algorithm no. 2
 *
 * To compile: gcc -Wall bucket_sort.c -o bucket_sort -fopenmp -std=c99
 *
 * To execute: ./bucket_sort no_elements no_threads no_buckets max_element
 * For instance: ./bucket_sort 10000000 4 100 5000
 */

#define _XOPEN_SOURCE

#include <omp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Bucket {
    int count;
    int* values;
};

void print_array(const int* array, const int array_size) {
    for (int i = 0; i < array_size; i++)
        printf("%i\n", array[i]);
}

void print_max_min_bucket(struct Bucket* buckets, const int no_buckets) {
    int max = 0;
    int min = 1000000;
    for(int i = 0; i < no_buckets; i++) {
        if (buckets[i].count > max)
            max = buckets[i].count;
        else if (buckets[i].count < min)
            min = buckets[i].count;
    }
    printf("Min bucket: %d, Max bucket: %d\n", min, max);
}

bool is_sorted(const int* array, const int array_size) {
    for (int i = 0; i < array_size - 1; i++) {
        if (array[i] > array[i + 1])
            return false;
    }
    return true;
}

int compare_integers(const void* first, const void* second) {
    int x = *((int*) first);
    int y = *((int*) second);

    if (x == y)
        return 0;
    else if (x > y)
        return 1;
    return -1;
}

double array_filler(int* array, const int array_size, const int max_element) {
    unsigned short int xi[3];
    double start_time = omp_get_wtime();
    #pragma omp parallel shared(array) private(xi)
    {
        unsigned seed = (unsigned) (time(NULL)) ^ omp_get_thread_num();
        xi[0] = seed * 1;
        xi[1] = seed * 2;
        xi[2] = seed * 4;
        #pragma omp for
        for(int i = 0; i < array_size; i++) {
            array[i] = (int) (erand48(xi) * max_element);
        }
    }
    return omp_get_wtime() - start_time;
}

double bucket_inserter(const int* array, const int array_size, struct Bucket* buckets, const double bucket_size, omp_lock_t* bucket_locks) {
    double start_time = omp_get_wtime();
    #pragma omp parallel for shared(array, buckets)
    for (int i = 0; i < array_size; i++) {
        int number = array[i];
        int bucket_index = (int) (number / bucket_size);
        omp_set_lock(&bucket_locks[bucket_index]);
        buckets[bucket_index].values[buckets[bucket_index].count++] = number;
        omp_unset_lock(&bucket_locks[bucket_index]);
    }
    return omp_get_wtime() - start_time;
}

double bucket_sorter(struct Bucket* buckets, const int no_buckets) {
    double start_time = omp_get_wtime();
    #pragma omp parallel for shared(buckets)
    for (int i = 0; i < no_buckets; i++) {
        qsort(buckets[i].values, buckets[i].count, sizeof(int), &compare_integers);
    }
    return omp_get_wtime() - start_time;
}

double array_inserter(int* array, struct Bucket* buckets, const int no_buckets, int* start_inserting_indexes) {
    double start_time = omp_get_wtime();

    int accumulator = 0;
    start_inserting_indexes[0] = accumulator;
    for (int i = 1; i < no_buckets; i++) {
        accumulator += buckets[i - 1].count;
        start_inserting_indexes[i] = accumulator;
    }

    #pragma omp parallel for shared(array, buckets, start_inserting_indexes)
    for (int i = 0; i < no_buckets; i++) {
        for (int j = 0; j < buckets[i].count; j++) {
            array[j + start_inserting_indexes[i]] = buckets[i].values[j];
        }
    }

    return omp_get_wtime() - start_time;
}

int main(int argc, char** argv) {

    if (argc != 5) {
        printf("Number of arguments have to be equal 4!\n");
        return -1;
    }

    int no_elements = atoi(argv[1]);    // number of elements
    int no_threads = atoi(argv[2]);     // number of threads
    int no_buckets = atoi(argv[3]);     // number of buckets
    int max_element = atoi(argv[4]);    // value of a maximum element possible + 1
    double bucket_range = (double) max_element / (double) no_buckets;   // range of buckets
    int* array = malloc(no_elements * sizeof(int));     // our array of numbers

    double start_entire_time;
    double pause_time;
    double continue_time;
    double time_taken;

    omp_set_num_threads(no_threads);

    start_entire_time = omp_get_wtime();

    time_taken = array_filler(array, no_elements, max_element);
    printf("Filling array time taken: %lf [s]\n", time_taken);

    pause_time = omp_get_wtime();

    // Bucket initialization. We do not want to measure time taken during this process, because it is memory allocation
    struct Bucket* buckets = malloc(no_buckets * sizeof(struct Bucket));    // buckets
    omp_lock_t* bucket_locks = malloc(no_buckets * sizeof(omp_lock_t));     // locks for buckets
    int* bucket_sizes = calloc(no_buckets, sizeof(int));                    // array with maximum size of each bucket
    for(int i = 0; i < no_elements; i++) {
        int bucket_index = (int) (array[i] / bucket_range);
        bucket_sizes[bucket_index]++;
    }
    for (int i = 0; i < no_buckets; i++) {
        buckets[i].count = 0;
        buckets[i].values = malloc(bucket_sizes[i] * sizeof(int));
        omp_init_lock(&bucket_locks[i]);
    }

    int* start_inserting_indexes = malloc(no_buckets * sizeof(int));        // offset of buckets (from which index we
                                                                            // should start inserting our bucket
                                                                            // into array after sorting

    continue_time = omp_get_wtime();

    time_taken = bucket_inserter(array, no_elements, buckets, bucket_range, bucket_locks);
    printf("Inserting into buckets time taken: %lf [s]\n", time_taken);

    time_taken = bucket_sorter(buckets, no_buckets);
    printf("Sorting buckets time taken: %lf [s]\n", time_taken);

    time_taken = array_inserter(array, buckets, no_buckets, start_inserting_indexes);
    printf("Inserting into array time taken: %lf [s]\n", time_taken);

    printf("Entire process took: %lf [s]\n", omp_get_wtime() - continue_time + pause_time - start_entire_time);

    //print_array(array, no_elements);
    //print_max_min_bucket(buckets, no_buckets);

    if (is_sorted(array, no_elements))
        printf("Array is sorted\n");
    else
        printf("Array is not sorted\n");

    free(array);
    for (int i = 0; i < no_buckets; i++) {
        free(buckets[i].values);
        omp_destroy_lock(&bucket_locks[i]);
    }
    free(buckets);
    free(start_inserting_indexes);

    return 1;
}
