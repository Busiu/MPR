// Matrix multiplication by parts
// Elements stored in row-major order

using namespace std;
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "helper_timer.h"
#define BLOCK_SIZE 16

typedef struct
{	int width;
	int height;
	float *elements;
} Matrix;

// Forward declaration of matrix mult
__global__ void MatMulKernel (const Matrix, const Matrix, Matrix);

// Host code
void MatMulGPU(const Matrix A, const Matrix B, Matrix C)
{
	StopWatchInterface *timer;

	// Load matrices A and B to device memory
	Matrix d_A;
	d_A.width = A.width; d_A.height = A.height;
	size_t size = A.width * A.height * sizeof(float);
	cudaMalloc((void**) &d_A.elements, size);
	cudaMemcpy(d_A.elements, A.elements, size, cudaMemcpyHostToDevice);
	
	Matrix d_B;
	d_B.width = B.width; d_B.height = B.height;
	size = B.width * B.height * sizeof(float);
	cudaMalloc((void**) &d_B.elements, size);
	cudaMemcpy(d_B.elements, B.elements, size, cudaMemcpyHostToDevice);
	
	// allocate C in device
	Matrix d_C;
	d_C.width = C.width; d_C.height = C.height;
	size = d_C.width * d_C.height * sizeof(float);
	cudaMalloc((void**) &d_C.elements, size);

	// call kernel
        dim3 dimBlock(100); // define the block size (what is the best value?) 
        dim3 dimGrid(1000); //  choose grid size depending on problem size 
		
		
	timer = NULL;
	sdkCreateTimer(&timer);
	sdkResetTimer(&timer);
	sdkStartTimer(&timer);	
	MatMulKernel<<<dimGrid, dimBlock>>>(d_A, d_B, d_C);
	cudaThreadSynchronize();
	sdkStopTimer(&timer);
    printf("GPU Time - %f ms\n", sdkGetTimerValue(&timer));
    sdkDeleteTimer(&timer);
	
	// copy C to host
	cudaMemcpy(C.elements, d_C.elements, size, cudaMemcpyDeviceToHost);
	
	// free device memory
	cudaFree(d_A.elements);
	cudaFree(d_B.elements);
	cudaFree(d_C.elements);
}

void MatMulCPU(Matrix A, Matrix B, Matrix C) {
	StopWatchInterface *timer;

	timer = NULL;
	sdkCreateTimer(&timer);
	sdkResetTimer(&timer);
	sdkStartTimer(&timer);	

	for (int x = 0; x < A.width * A.height; x++) {
		float sum = 0;
		for (int i = 0; i < A.width; i++) {
			sum += A.elements[x / A.width + i] * B.elements[x % A.width + i  * A.width];
		}
		C.elements[x] = sum;
	}

	sdkStopTimer(&timer);
    printf("CPU Time - %f ms\n", sdkGetTimerValue(&timer));
    sdkDeleteTimer(&timer);
}

// matrix multiplication kernel
__global__ void MatMulKernel(Matrix A, Matrix B, Matrix C)
{	
	int tid = blockIdx.x * blockDim.x + threadIdx.x;
	int offset = gridDim.x * blockDim.x;
    for (int x = tid; x < A.width * A.height; x += offset) {
		float sum = 0;
		for (int i = 0; i < A.width; i++) {
			sum += A.elements[(x / A.width) + i] * B.elements[(x % A.width) + (i  * A.width)];
		}
		C.elements[x] = sum;
	}
}

int main(int argc, char * const argv[])
{	
	int Height = 512;
	int Width = 1024;

	Matrix A;
	Matrix B;
	Matrix C;

	A.height = Height;
	B.height = Width;
	C.height = Height;

	A.width = Width;
	B.width = Height;
	C.width = Height;

	A.elements = new float[Width*Height];
	B.elements = new float[Width*Height];
	C.elements = new float[Width*Height];

	// fill matrices
	std::ifstream A_input;
	std::ifstream B_input;
	A_input.open("A_512x1024.txt");
	B_input.open("B_512x1024.txt");
	
	float a, b;
	A_input >> a;	
	B_input >> b;	
	int i = 0;
	while (!A_input.eof())
	{	A.elements[i] = a;
		B.elements[i] = b;
		A_input >> a;	
		B_input >> b;	
		i += 1;
	}
	A_input.close();
	B_input.close();

	MatMulGPU(A, B, C);
	//MatMulCPU(A, B, C);

	/*
	std::ofstream C_output;
	C_output.open("C.txt");
	for (int i=0; i<Width; i++)
	{	for (int j=0; j<Width; j++)
			C_output<<C.elements[i*Width+j]<<"\t";
		C_output<<endl;
	}
	*/
}
	
