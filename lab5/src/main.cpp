#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <chrono>
#include <iostream>

#include "mandelbrotSerial.h"
#include "mandelbrotThread.h"

// Вспомогательная функция для сохранения картинки (PPM)
void writePPMImage(int* data, int width, int height, const char* filename, int maxIterations) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Error: Could not open %s for writing\n", filename);
        return;
    }
    fprintf(fp, "P6\n%d %d\n255\n", width, height);

    for (int i = 0; i < width * height; ++i) {
        unsigned char color[3];
        // Простое раскрашивание на основе количества итераций
        int val = data[i];
        if (val == maxIterations) {
            color[0] = 0; color[1] = 0; color[2] = 0; // Черный внутри множества
        }
        else {
            color[0] = (val * 5) % 256;
            color[1] = (val * 10) % 256;
            color[2] = (val * 20) % 256;
        }
        fwrite(color, 1, 3, fp);
    }
    fclose(fp);
    printf("Saved image to %s\n", filename);
}

bool verifyResult(int* gold, int* result, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (gold[i * width + j] != result[i * width + j]) {
                printf("Mismatch : [%d][%d], Expected : %d, Actual : %d\n",
                    i, j, gold[i * width + j], result[i * width + j]);
                return 0;
            }
        }
    }
    return 1;
}

int main(int argc, char** argv) {
    const unsigned int width = 1600;
    const unsigned int height = 1200;
    const int maxIterations = 256;
    int numThreads = 2; // По умолчанию 2 потока

    // Если передали аргумент командной строки (например, mandelbrot.exe 4), берем оттуда
    if (argc > 1) {
        numThreads = atoi(argv[1]);
    }

    float x0 = -2, x1 = 1, y0 = -1, y1 = 1;

    int* output_serial = new int[width * height];
    int* output_thread = new int[width * height];

    // --- СЕРИЙНАЯ ВЕРСИЯ ---
    double minSerial = 1e30;
    for (int i = 0; i < 3; ++i) {
        memset(output_serial, 0, width * height * sizeof(int));
        auto start = std::chrono::high_resolution_clock::now();

        mandelbrotSerial(x0, y0, x1, y1, width, height, 0, height, maxIterations, output_serial);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        minSerial = std::min(minSerial, elapsed.count());
    }
    printf("[mandelbrot serial]:\t\t[%.3f] ms\n", minSerial * 1000);
    writePPMImage(output_serial, width, height, "mandelbrot-serial.ppm", maxIterations);

    // --- ПАРАЛЛЕЛЬНАЯ ВЕРСИЯ ---
    double minThread = 1e30;
    for (int i = 0; i < 3; ++i) {
        memset(output_thread, 0, width * height * sizeof(int));
        auto start = std::chrono::high_resolution_clock::now();

        // Вызов функции из mandelbrotThread.cpp
        mandelbrotThread(numThreads, x0, y0, x1, y1, width, height, maxIterations, output_thread);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        minThread = std::min(minThread, elapsed.count());
    }
    printf("[mandelbrot thread]:\t\t[%.3f] ms\n", minThread * 1000);
    writePPMImage(output_thread, width, height, "mandelbrot-thread.ppm", maxIterations);

    // --- ПРОВЕРКА ---
    if (!verifyResult(output_serial, output_thread, width, height)) {
        printf("Error : Output from threads does not match serial output\n");
        delete[] output_serial;
        delete[] output_thread;
        return 1;
    }

    printf("\t\t\t\t(%.2fx speedup from %d threads)\n", minSerial / minThread, numThreads);

    delete[] output_serial;
    delete[] output_thread;
    return 0;
}