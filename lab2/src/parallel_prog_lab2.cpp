#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <omp.h>
using namespace std;

int main() {
    system("chcp 1251 > nul");

    ifstream file1("input/matrix_a.txt");
    int m = 0;
    file1 >> m;
    vector<vector<double>> A;
    A.resize(m);
    for (int i = 0; i < m; i++) {
        A[i].resize(m);
    }
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            file1 >> A[i][j];
        }
    }
    file1.close();

    ifstream file2("input/matrix_b.txt");
    int n = 0;
    file2 >> n;
    vector<vector<double>> B;
    B.resize(n);
    for (int i = 0; i < n; i++) {
        B[i].resize(n);
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            file2 >> B[i][j];
        }
    }
    file2.close();

    cout << "Size matrix A: " << m << "x" << m << endl;
    cout << "Size matrix B: " << n << "x" << n << endl;
    cout << "Threads: " << omp_get_max_threads() << endl;

    vector<vector<double>> C;
    C.resize(m);
    for (int i = 0; i < m; i++) {
        C[i].resize(n);
    }

    auto start = chrono::high_resolution_clock::now();

#pragma omp parallel for
    for (int i = 0; i < m; i++) {
        for (int k = 0; k < n; k++) {
            for (int j = 0; j < n; j++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    auto end = chrono::high_resolution_clock::now();
    double time_sec = chrono::duration<double>(end - start).count();

    cout << "mult is completed :)" << endl;
    cout << "Size matrix C: " << m << "x" << n << endl;
    cout << "time: " << time_sec << " sec" << endl;

    int total_elements = m * m + n * n;
    cout << "Volume: " << total_elements << " elements" << endl;

    ofstream out("output/result.txt");
    out << m << endl;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            out << C[i][j] << " ";
        }
        out << endl;
    }
    out.close();

    ofstream info("output/info.txt");
    info << "Matrix A size: " << m << "x" << m << endl;
    info << "Matrix B size: " << n << "x" << n << endl;
    info << "Threads: " << omp_get_max_threads() << endl;
    info << "Execution time: " << time_sec << " sec" << endl;
    info << "Task volume: " << total_elements << " elements" << endl;
    info.close();

    cout << "Info saved to output/info.txt" << endl;
    cout << "The work has been completed!" << endl;

    return 0;
}