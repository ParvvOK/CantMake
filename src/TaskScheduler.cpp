#include "TaskScheduler.h"
using namespace std;

namespace cantmake {

void TaskScheduler::addTask(const BuildTask& task) {
    {
        lock_guard<mutex> lock(queueMutex);
        tasks.push(task);
    }
    cv.notify_one();
}

bool TaskScheduler::getTask(BuildTask& task) {
    unique_lock<mutex> lock(queueMutex);
    
    cv.wait(lock, [this]() {
        return !tasks.empty() || finished;
    });

    if (tasks.empty() && finished) {
        return false;
    }

    task = tasks.front();
    tasks.pop();
    return true;
}

void TaskScheduler::setFinished() {
    {
        lock_guard<mutex> lock(queueMutex);
        finished = true;
    }
    cv.notify_all();
}

}