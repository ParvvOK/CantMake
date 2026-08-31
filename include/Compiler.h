#pragma once
#include "ProjectConfig.h"
#include "DependencyGraph.h"
#include <vector>
#include <string>
#include <mutex>
using namespace std;

namespace cantmake {

class Compiler {
public:
    explicit Compiler(const ProjectConfig& config);

    bool compile(FileNode* node);

    bool link(const vector<FileNode*>& allNodes);

private:
    string getObjectFileName(const string& sourceFile) const;
    string buildCompileCommand(const string& sourceFile, const string& objectFile) const;
    string buildLinkCommand(const vector<FileNode*>& allNodes) const;

    ProjectConfig config;
    mutex printMutex;
};

}