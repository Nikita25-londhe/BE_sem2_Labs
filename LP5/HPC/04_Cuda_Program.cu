%%writefile program.cu
#include<iostream>
#include<stdio.h>
using namespace std;

// -------- VECTOR ADDITION --------
__global__ void vecAdd(float* A, float* B, float* C, int n)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if(i < n)
        C[i] = A[i] + B[i];
}

// -------- MATRIX MULTIPLICATION --------
__global__ void matMul(float* A, float* B, float* C, int n)
{
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if(row < n && col < n)
    {
        float sum = 0;
        for(int k = 0; k < n; k++)
        {
            sum += A[row*n + k] * B[k*n + col];
        }
        C[row*n + col] = sum;  
    }
}

int main()
{
    // -------- VECTOR ADDITION --------
    int n = 4;
    float A[] = {1,2,3,4};
    float B[] = {5,6,7,8};
    float C[4];

    float *d_A, *d_B, *d_C;
    size_t size = n * sizeof(float);

    cudaMalloc(&d_A, size);
    cudaMalloc(&d_B, size);
    cudaMalloc(&d_C, size);

    cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);

    vecAdd<<<1, n>>>(d_A, d_B, d_C, n);

    cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);

    cout << "Vector Addition Result:\n";
    for(int i=0;i<n;i++)
        cout << C[i] << " ";

    // -------- MATRIX MULTIPLICATION --------
    int N = 2;
    float matA[] = {1,2,3,4};
    float matB[] = {5,6,7,8};
    float matC[4];

    float *d_matA, *d_matB, *d_matC;
    size_t size2 = N*N*sizeof(float);

    cudaMalloc(&d_matA, size2);
    cudaMalloc(&d_matB, size2);
    cudaMalloc(&d_matC, size2);

    cudaMemcpy(d_matA, matA, size2, cudaMemcpyHostToDevice);
    cudaMemcpy(d_matB, matB, size2, cudaMemcpyHostToDevice);

    dim3 threads(2,2);   // threads per block
    dim3 blocks(1,1);    // number of blocks

    matMul<<<blocks, threads>>>(d_matA, d_matB, d_matC, N);

    cudaMemcpy(matC, d_matC, size2, cudaMemcpyDeviceToHost);

    cout << "\n\nMatrix Multiplication Result:\n";
    for(int i=0;i<4;i++)
        cout << matC[i] << " ";


    return 0;
}