#include "ThreadPool.h"
#include <iostream>

namespace cantmake {

ThreadPool::ThreadPool(size_t numThreads, TaskScheduler& scheduler, Compiler& compiler)
    : numThreads(numThreads), scheduler(scheduler), compiler(compiler) {}

ThreadPool::~ThreadPool() {
    wait();
}

void ThreadPool::start() {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back(&ThreadPool::workerLoop, this);
    }
}   

void ThreadPool::wait() {
    for (auto& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
    workers.clear();
}

bool ThreadPool::hasFailed() const {
    return failed.load();
}

void ThreadPool::workerLoop() {
    while (true) {
        if (failed.load()) {
            break;
        }

        BuildTask task;
        if (!scheduler.getTask(task)) {
            break;
        }

        if (!compiler.compile(task.node)) {
            failed.store(true);
        }
    }
}

}