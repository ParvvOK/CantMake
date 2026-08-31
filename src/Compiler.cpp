#include "Compiler.h"
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <sstream>
using namespace std;

namespace cantmake {

Compiler::Compiler(const ProjectConfig& config) : config(config) {
    filesystem::create_directories(config.projectRoot / "build");
}

bool Compiler::compile(FileNode* node) {
    string objectFile = getObjectFileName(node->filename);
    string command = buildCompileCommand(node->filename, objectFile);

    {
        lock_guard<mutex> lock(printMutex);
        cout << "Compiling: " << node->filename << endl;
    }

    int result = system(command.c_str());
    
    if (result != 0) {
        lock_guard<mutex> lock(printMutex);
        cerr << "Error compiling " << node->filename << endl;
        return false;
    }
    
    return true;
}

bool Compiler::link(const vector<FileNode*>& allNodes) {
    string command = buildLinkCommand(allNodes);

    cout << "Linking: " << config.outputExecutable << endl;

    int result = system(command.c_str());
    
    if (result != 0) {
        cerr << "Error during linking" << endl;
        return false;
    }
    
    return true;
}

string Compiler::getObjectFileName(const string& sourceFile) const {
    filesystem::path p(sourceFile);
    string basename = p.stem().string();
    return (config.projectRoot / "build" / (basename + ".o")).string();
}

string Compiler::buildCompileCommand(const string& sourceFile, const string& objectFile) const {
    stringstream cmd;
    cmd << config.compiler << " ";
    
    if (!config.cppStandard.empty()) {
        cmd << "-std=" << config.cppStandard << " ";
    }

    for (const auto& inc : config.includeDirectories) {
        cmd << "-I" << inc << " ";
    }

    cmd << "-c " << sourceFile << " -o " << objectFile;
    return cmd.str();
}

string Compiler::buildLinkCommand(const vector<FileNode*>& allNodes) const {
    stringstream cmd;
    cmd << config.compiler << " ";

    for (const auto& node : allNodes) {
        cmd << getObjectFileName(node->filename) << " ";
    }

    cmd << "-o " << config.outputExecutable;
    return cmd.str();
}

}