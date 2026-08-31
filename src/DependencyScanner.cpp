#include "DependencyScanner.h"
#include "Utils.h"
#include <fstream>
#include <filesystem>
#include <iostream>
using namespace std;

namespace cantmake {

DependencyScanner::DependencyScanner(const ProjectConfig& config, DependencyGraph& graph)
    : config(config), graph(graph) {}

void DependencyScanner::scan() {
    unordered_set<string> visited;
    
    for (const auto& sourceFile : config.sourceFiles) {
        FileNode* node = graph.getOrCreateNode(sourceFile);
        scanFile(sourceFile, node, visited);
    }
}

void DependencyScanner::scanFile(const string& filename, FileNode* node, unordered_set<string>& visited) {
    if (visited.find(filename) != visited.end()) {
        return;
    }
    visited.insert(filename);

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Warning: Could not open file for dependency scanning: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        line = utils::trim(line);
        if (line.rfind("#include", 0) == 0) {
            size_t firstQuote = line.find('"');
            if (firstQuote != string::npos) {
                size_t secondQuote = line.find('"', firstQuote + 1);
                if (secondQuote != string::npos) {
                    string includeName = line.substr(firstQuote + 1, secondQuote - firstQuote - 1);
                    string resolvedPath = resolveIncludePath(includeName);
                    
                    if (!resolvedPath.empty()) {
                        FileNode* depNode = graph.getOrCreateNode(resolvedPath);
                        
                        node->dependencies.push_back(depNode);
                        depNode->dependents.push_back(node);
                        
                        scanFile(resolvedPath, depNode, visited);
                    }
                }
            }
        }
    }
}

string DependencyScanner::resolveIncludePath(const string& includeName) const {
    if (filesystem::exists(includeName)) {
        return includeName;
    }

    for (const auto& dir : config.includeDirectories) {
        filesystem::path p = filesystem::path(dir) / includeName;
        if (filesystem::exists(p)) {
            return p.string();
        }
    }

    return "";
}

}