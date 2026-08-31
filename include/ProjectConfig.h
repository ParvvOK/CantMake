#pragma once
#include <filesystem>
#include <string>
#include <vector>
using namespace std;

namespace cantmake {

struct ProjectConfig {
    string projectName;
    string compiler;
    string cppStandard;
    string outputExecutable;
    vector<string> sourceFiles;
    vector<string> includeDirectories;
    filesystem::path projectRoot;

    void resolvePaths();
};

}