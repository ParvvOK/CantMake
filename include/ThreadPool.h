#pragma once
#include "TaskScheduler.h"
#include "Compiler.h"
#include <vector>
#include <thread>
#include <atomic>
using namespace std;

namespace cantmake {

class ThreadPool {
public:
    ThreadPool(size_t numThreads, TaskScheduler& scheduler, Compiler& compiler);
    ~ThreadPool();

    void start();
    void wait();

    bool hasFailed() const;

private:
    void workerLoop();

    size_t numThreads;
    TaskScheduler& scheduler;
    Compiler& compiler;
    vector<thread> workers;
    atomic<bool> failed{false};
};

}