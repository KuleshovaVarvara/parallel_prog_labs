#include <mpi.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = 0;
    vector<double> A, B, C, C_serial;

    // 1. Чтение данных (только процесс 0)
    if (rank == 0) {
        ifstream fileA("input/matrix_a.txt");
        ifstream fileB("input/matrix_b.txt");

        if (!fileA.is_open() || !fileB.is_open()) {
            cerr << "Error: cannot open files in input/" << endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        fileA >> N;
        A.resize(N * N);
        B.resize(N * N);
        C.resize(N * N, 0.0);
        C_serial.resize(N * N, 0.0);

        for (int i = 0; i < N * N; ++i) fileA >> A[i];
        for (int i = 0; i < N * N; ++i) fileB >> B[i];
    }

    // 2. Рассылаем N и B всем процессам
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank != 0) B.resize(N * N);
    MPI_Bcast(B.data(), N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // 3. Вычисляем границы строк (как у одногруппника)
    int start_row = N * rank / size;
    int end_row = N * (rank + 1) / size;
    int local_rows = end_row - start_row;
    int local_elements = local_rows * N;

    vector<double> local_A(local_elements);
    vector<double> local_C(local_elements, 0.0);

    // 4. Готовим массивы counts и displs для Scatterv/Gatherv
    vector<int> counts(size), displs(size);
    for (int i = 0; i < size; ++i) {
        int s = N * i / size;
        int e = N * (i + 1) / size;
        counts[i] = (e - s) * N;
        displs[i] = s * N;
    }

    // 5. Замер времени
    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();

    // 6. Рассылка строк матрицы A
    MPI_Scatterv(A.data(), counts.data(), displs.data(), MPI_DOUBLE,
        local_A.data(), local_elements, MPI_DOUBLE,
        0, MPI_COMM_WORLD);

    // 7. Умножение (MPI)
    for (int i = 0; i < local_rows; ++i) {
        for (int k = 0; k < N; ++k) {
            double aik = local_A[i * N + k];
            for (int j = 0; j < N; ++j) {
                local_C[i * N + j] += aik * B[k * N + j];
            }
        }
    }

    // 8. Сбор результата
    MPI_Gatherv(local_C.data(), local_elements, MPI_DOUBLE,
        C.data(), counts.data(), displs.data(), MPI_DOUBLE,
        0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();

    // 9. Проверка на процессе 0 (считаем эталон последовательно)
    if (rank == 0) {
        // Считаем эталон (последовательно)
        for (int i = 0; i < N; ++i) {
            for (int k = 0; k < N; ++k) {
                double aik = A[i * N + k];
                for (int j = 0; j < N; ++j) {
                    C_serial[i * N + j] += aik * B[k * N + j];
                }
            }
        }

        // Сравниваем
        double max_error = 0.0;
        for (int i = 0; i < N * N; ++i) {
            double diff = fabs(C[i] - C_serial[i]);
            if (diff > max_error) max_error = diff;
        }

        // Выводим результат
        cout << "N = " << N << ", Processors = " << size << endl;
        cout << "Time: " << fixed << setprecision(6) << (end_time - start_time) << " sec" << endl;
        cout << "Max error: " << max_error << endl;

        if (max_error < 1e-9) {
            cout << "✅ VERIFICATION PASSED!" << endl;
        }
        else {
            cout << "❌ VERIFICATION FAILED!" << endl;
        }

        // Записываем отчёт
        ofstream report("output/matrix_report.txt");
        report << "Matrix size: " << N << endl;
        report << "Processors: " << size << endl;
        report << "Time: " << fixed << setprecision(6) << (end_time - start_time) << " sec" << endl;
        report << "Max error: " << max_error << endl;
        report << (max_error < 1e-9 ? "PASSED" : "FAILED") << endl;
        report.close();

        // Записываем результат
        ofstream out("output/result.txt");
        out << N << endl;
        out << setprecision(15);
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                out << C[i * N + j] << " ";
            }
            out << endl;
        }
    }

    MPI_Finalize();
    return 0;
}