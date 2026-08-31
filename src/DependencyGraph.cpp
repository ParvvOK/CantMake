#include "DependencyGraph.h"
using namespace std;

namespace cantmake {

FileNode* DependencyGraph::getOrCreateNode(const string& filename) {
    auto it = nodes.find(filename);
    if (it != nodes.end()) {
        return &it->second;
    }
    
    FileNode& node = nodes[filename];
    node.filename = filename;
    return &node;
}

}