#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;

// ---------------- VECTOR ADDITION (GPU) ----------------
__global__ void vectorAdd(int *a, int *b, int *c, int n)
{
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    if (idx < n)
        c[idx] = a[idx] + b[idx];
}

// ---------------- VECTOR ADDITION (CPU) ----------------
void vectorAddCPU(int *a, int *b, int *c, int n)
{
    for (int i = 0; i < n; i++)
        c[i] = a[i] + b[i];
}

// ---------------- MATRIX MULTIPLICATION (GPU) ----------------
__global__ void matrixMul(int *A, int *B, int *C, int N)
{
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < N && col < N)
    {
        int sum = 0;
        for (int k = 0; k < N; k++)
            sum += A[row * N + k] * B[k * N + col];

        C[row * N + col] = sum;
    }
}

// ---------------- MATRIX MULTIPLICATION (CPU) ----------------
void matrixMulCPU(int *A, int *B, int *C, int N)
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
        {
            int sum = 0;
            for (int k = 0; k < N; k++)
                sum += A[i * N + k] * B[k * N + j];

            C[i * N + j] = sum;
        }
}

// ---------------- MAIN ----------------
int main()
{
    // -------- VECTOR ADDITION --------
    int n = 1 << 20; // ~1 million elements

    int *h_a = new int[n];
    int *h_b = new int[n];
    int *h_c_cpu = new int[n];
    int *h_c_gpu = new int[n];

    for (int i = 0; i < n; i++)
    {
        h_a[i] = rand() % 100;
        h_b[i] = rand() % 100;
    }

    // CPU Time
    auto start = high_resolution_clock::now();
    vectorAddCPU(h_a, h_b, h_c_cpu, n);
    auto end = high_resolution_clock::now();
    cout << "Vector CPU Time: "
         << duration_cast<milliseconds>(end - start).count() << " ms\n";

    // GPU Memory
    int *d_a, *d_b, *d_c;
    cudaMalloc(&d_a, n * sizeof(int));
    cudaMalloc(&d_b, n * sizeof(int));
    cudaMalloc(&d_c, n * sizeof(int));

    cudaMemcpy(d_a, h_a, n * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, n * sizeof(int), cudaMemcpyHostToDevice);

    // GPU Time
    start = high_resolution_clock::now();

    vectorAdd<<<(n + 255) / 256, 256>>>(d_a, d_b, d_c, n);
    cudaDeviceSynchronize();

    end = high_resolution_clock::now();

    cudaMemcpy(h_c_gpu, d_c, n * sizeof(int), cudaMemcpyDeviceToHost);

    cout << "Vector GPU Time: "
         << duration_cast<milliseconds>(end - start).count() << " ms\n\n";

    // -------- MATRIX MULTIPLICATION --------
    int N = 512; // matrix size

    int *A = new int[N * N];
    int *B = new int[N * N];
    int *C_cpu = new int[N * N];
    int *C_gpu = new int[N * N];

    for (int i = 0; i < N * N; i++)
    {
        A[i] = rand() % 10;
        B[i] = rand() % 10;
    }

    // CPU Time
    start = high_resolution_clock::now();
    matrixMulCPU(A, B, C_cpu, N);
    end = high_resolution_clock::now();

    cout << "Matrix CPU Time: "
         << duration_cast<milliseconds>(end - start).count() << " ms\n";

    // GPU Memory
    int *d_A, *d_B, *d_C;
    int size = N * N * sizeof(int);

    cudaMalloc(&d_A, size);
    cudaMalloc(&d_B, size);
    cudaMalloc(&d_C, size);

    cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);

    dim3 threads(16, 16);
    dim3 blocks((N + 15) / 16, (N + 15) / 16);

    // GPU Time
    start = high_resolution_clock::now();

    matrixMul<<<blocks, threads>>>(d_A, d_B, d_C, N);
    cudaDeviceSynchronize();

    end = high_resolution_clock::now();

    cudaMemcpy(C_gpu, d_C, size, cudaMemcpyDeviceToHost);

    cout << "Matrix GPU Time: "
         << duration_cast<milliseconds>(end - start).count() << " ms\n";

    // Cleanup
    delete[] h_a;
    delete[] h_b;
    delete[] h_c_cpu;
    delete[] h_c_gpu;
    delete[] A;
    delete[] B;
    delete[] C_cpu;
    delete[] C_gpu;

    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}