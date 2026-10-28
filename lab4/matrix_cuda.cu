#include <cuda_runtime.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <cmath>

using namespace std;

// CUDA-ядро для умножения матриц
__global__ void matrixMulKernel(const double* A, const double* B, double* C, int N) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < N && col < N) {
        double sum = 0.0;
        for (int k = 0; k < N; ++k) {
            sum += A[row * N + k] * B[k * N + col];
        }
        C[row * N + col] = sum;
    }
}

int main() {
    // Чтение N и матриц
    ifstream fileA("input/matrix_a.txt");
    ifstream fileB("input/matrix_b.txt");
    if (!fileA.is_open() || !fileB.is_open()) {
        cerr << "Error: cannot open files in input/" << endl;
        return 1;
    }

    int N;
    fileA >> N;

    vector<double> h_A(N * N);
    vector<double> h_B(N * N);
    vector<double> h_C(N * N, 0.0);

    for (int i = 0; i < N * N; ++i) fileA >> h_A[i];
    for (int i = 0; i < N * N; ++i) fileB >> h_B[i];

    // Выделение памяти на GPU
    double *d_A, *d_B, *d_C;
    size_t bytes = N * N * sizeof(double);
    cudaMalloc(&d_A, bytes);
    cudaMalloc(&d_B, bytes);
    cudaMalloc(&d_C, bytes);

    // Копирование данных на GPU
    cudaMemcpy(d_A, h_A.data(), bytes, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B.data(), bytes, cudaMemcpyHostToDevice);

    // Конфигурация сетки блоков
    dim3 blockSize(16, 16);
    dim3 gridSize((N + 15) / 16, (N + 15) / 16);

    // Замер времени
    auto start = chrono::high_resolution_clock::now();

    matrixMulKernel<<<gridSize, blockSize>>>(d_A, d_B, d_C, N);

    auto end = chrono::high_resolution_clock::now();
    double elapsed = chrono::duration<double>(end - start).count();

    // Копирование результата обратно
    cudaMemcpy(h_C.data(), d_C, bytes, cudaMemcpyDeviceToHost);

    // Освобождение памяти
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    cout << "N = " << N << endl;
    cout << "Time: " << elapsed << " sec" << endl;

    // Запись результата
    ofstream out("output/result.txt");
    out << N << endl;
    out.precision(15);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            out << h_C[i * N + j] << " ";
        }
        out << endl;
    }

    ofstream info("output/info.txt");
    info << "N = " << N << endl;
    info << "Time: " << elapsed << " sec" << endl;
    info << "Block size: 16x16" << endl;
    info << "Grid size: " << gridSize.x << "x" << gridSize.y << endl;

    return 0;
}