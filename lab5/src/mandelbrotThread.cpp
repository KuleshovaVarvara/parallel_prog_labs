#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <algorithm>
#include "mandelbrotSerial.h"
#include "mandelbrotThread.h"

void workerThreadStart(WorkerArgs* const args) {
    const int threadId = args->threadId;
    const int numThreads = args->numThreads;
    const int width = static_cast<int>(args->width);
    const int height = static_cast<int>(args->height);

    for (int row = threadId; row < height; row += numThreads) {
        mandelbrotSerial(
            args->x0, args->y0, args->x1, args->y1,
            width, height,
            row, 1,  
            args->maxIterations,
            args->output
        );
    }
}

void mandelbrotThread(
    int numThreads,
    float x0, float y0, float x1, float y1,
    int width, int height,
    int maxIterations, int output[])
{
    const int MAX_THREADS = 32;
    std::thread workers[MAX_THREADS];
    WorkerArgs args[MAX_THREADS];

    for (int i = 0; i < numThreads; i++) {
        args[i].x0 = x0;
        args[i].y0 = y0;
        args[i].x1 = x1;
        args[i].y1 = y1;
        args[i].width = width;
        args[i].height = height;
        args[i].maxIterations = maxIterations;
        args[i].numThreads = numThreads;
        args[i].output = output;
        args[i].threadId = i;
    }

    workerThreadStart(&args[0]);

    for (int i = 1; i < numThreads; i++) {
        workers[i] = std::thread(workerThreadStart, &args[i]);
    }

    for (int i = 1; i < numThreads; i++) {
        workers[i].join();
    }
}