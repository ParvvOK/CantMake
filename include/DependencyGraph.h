#pragma once
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

namespace cantmake {

struct FileNode {
    string filename;
    vector<FileNode*> dependencies;
    vector<FileNode*> dependents;
    bool needsCompilation = false;
};

class DependencyGraph {
public:
    FileNode* getOrCreateNode(const string& filename);
 
    unordered_map<string, FileNode>& getNodes() { return nodes; }
    const unordered_map<string, FileNode>& getNodes() const { return nodes; }

private:
    unordered_map<string, FileNode> nodes;
};

}