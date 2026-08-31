#pragma once
#include "ProjectConfig.h"
#include "DependencyGraph.h"
#include <string>
#include <vector>
#include <unordered_set>
using namespace std;

namespace cantmake {

class DependencyScanner {
public:
    DependencyScanner(const ProjectConfig& config, DependencyGraph& graph);

    void scan();

private:
    void scanFile(const string& filename, FileNode* node, unordered_set<string>& visited);
    string resolveIncludePath(const string& includeName) const;

    const ProjectConfig& config;
    DependencyGraph& graph;
};

}