#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <chrono>
#include <iomanip>   // ← ДОБАВЛЕНО для setprecision

int main() {
    int N;
    std::cout << "Введите N: ";
    std::cin >> N;

    std::vector<double> A(N * N);
    std::vector<double> B(N * N);
    std::vector<double> C(N * N);

    std::srand(std::time(nullptr));

    for (int i = 0; i < N * N; ++i) {
        A[i] = (double)std::rand() / RAND_MAX;
        B[i] = (double)std::rand() / RAND_MAX;
    }

    for (int i = 0; i < N * N; ++i) {
        C[i] = 0.0;
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                C[i * N + j] += A[i * N + k] * B[k * N + j];
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double>(end - start).count();
    std::cout << "Время выполнения: " << elapsed << " секунд" << std::endl;

    long long operations = (long long)N * N * N;
    std::cout << "Объём задачи: " << operations << " операций" << std::endl;

    // Пишем сразу в папку PyCharm
    std::ofstream out("C:/Users/Варвара/PycharmProjects/WelcomeScreen/result.txt");

    out << N << std::endl;
    out << std::setprecision(15);   // ← 15 знаков точности

    // Матрица A
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            out << A[i * N + j] << " ";
        }
        out << std::endl;
    }

    // Матрица B
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            out << B[i * N + j] << " ";
        }
        out << std::endl;
    }

    // Матрица C
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            out << C[i * N + j] << " ";
        }
        out << std::endl;
    }

    out << "Время: " << elapsed << " секунд" << std::endl;
    out << "Объём задачи: " << operations << " операций" << std::endl;

    out.close();

    std::cout << "Умножение завершено!" << std::endl;
    std::cout << "Результат сохранён в файл result.txt" << std::endl;
    std::cout << "C[0][0] = " << C[0] << std::endl;
    if (N > 1) {
        std::cout << "C[0][1] = " << C[1] << std::endl;
        std::cout << "C[1][0] = " << C[N] << std::endl;
    }

    return 0;
}