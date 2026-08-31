#pragma once
#include "DependencyGraph.h"
#include <queue>
#include <mutex>
#include <condition_variable>
using namespace std;

namespace cantmake {

struct BuildTask {
    FileNode* node;
};

class TaskScheduler {
public:
    void addTask(const BuildTask& task);
    bool getTask(BuildTask& task);
    void setFinished();

private:
    queue<BuildTask> tasks;
    mutex queueMutex;
    condition_variable cv;
    bool finished = false;
};

}