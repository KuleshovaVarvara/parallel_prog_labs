#ifndef MANDELBROT_THREAD_H
#define MANDELBROT_THREAD_H

struct WorkerArgs {
    float x0, y0, x1, y1;
    unsigned int width;
    unsigned int height;
    int maxIterations;
    int threadId;
    int numThreads;
    int* output;   
};

void workerThreadStart(WorkerArgs* const args);

void mandelbrotThread(
    int numThreads,
    float x0, float y0, float x1, float y1,
    int width, int height,
    int maxIterations,
    int output[]   
);

#endif