#include <stdio.h>
#include <cuda.h>
#include <stdlib.h>
#include "helper_timer.h"

__global__ void addGPU(int* a, int* b, int* c, int N) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    int offset = gridDim.x * blockDim.x;
    for (int i = tid; i < N; i += offset)
        c[i] = a[i] + b[i];
}

void addCPU(int* a, int* b, int* c, int N) {
    for (int i = 0; i < N; i++)
        c[i] = a[i] + b[i];
}

bool isVectorsEqual(int* a, int* b, int N) {
    for (int i = 0; i < N; i++) {
        if (a[i] != b[i])
            return false;
    }
    return true;
}

int main(void) {
    int *a, *b, *c, *d;
    int *dev_a, *dev_b, *dev_c;
    int N, number_of_threads_per_block, number_of_blocks;
    StopWatchInterface *timer;

    printf("Please, enter the size of an array:\n");
    scanf("%d", &N);
    printf("Please, enter the number of threads per block:\n");
    scanf("%d", &number_of_threads_per_block);
    printf("Please, enter the number of blocks:\n");
    scanf("%d", &number_of_blocks);

    a = (int*) malloc(N * sizeof(int));
    b = (int*) malloc(N * sizeof(int));
    c = (int*) malloc(N * sizeof(int));
    d = (int*) malloc(N * sizeof(int));
    cudaMalloc((void**)&dev_a, N * sizeof(int));
    cudaMalloc((void**)&dev_b, N * sizeof(int));
    cudaMalloc((void**)&dev_c, N * sizeof(int));

    for (int i = 0; i < N; i++) {
        a[i] = i;
        b[i] = 2 * i;
    }

    // GPU ----------------------------------------------------------
    cudaMemcpy(dev_a, a, N * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_b, b, N * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_c, c, N * sizeof(int), cudaMemcpyHostToDevice);

    timer = NULL;
    sdkCreateTimer(&timer);
    sdkResetTimer(&timer);
    sdkStartTimer(&timer);

    addGPU <<<number_of_blocks, number_of_threads_per_block>>> (dev_a, dev_b, dev_c, N);
    cudaThreadSynchronize();
    
    sdkStopTimer(&timer);
    printf("GPU Time - %f ms\n", sdkGetTimerValue(&timer));
    sdkDeleteTimer(&timer);

    cudaMemcpy(c, dev_c, N * sizeof(int), cudaMemcpyDeviceToHost);
    // --------------------------------------------------------------
    
    // CPU ----------------------------------------------------------
    timer = NULL;
    sdkCreateTimer(&timer);
    sdkResetTimer(&timer);
    sdkStartTimer(&timer);

    addCPU(a, b, d, N);

    sdkStopTimer(&timer);
    printf("CPU Time - %f ms\n", sdkGetTimerValue(&timer));
    sdkDeleteTimer(&timer);
    // --------------------------------------------------------------

    if (isVectorsEqual(c, d, N))
        printf("Vectors are the same\n");
    else
        printf("Vectors are not the same\n");

    cudaFree(dev_a);
    cudaFree(dev_a);
    cudaFree(dev_a);
    free(a);
    free(b);
    free(c);
    free(d);

    return 0;
}