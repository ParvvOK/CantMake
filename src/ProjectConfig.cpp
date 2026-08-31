#include "ProjectConfig.h"
#include "Utils.h"
using namespace std;

namespace cantmake {

void ProjectConfig::resolvePaths() {
    auto resolve = [this](const string& path) {
        return utils::resolveProjectPath(projectRoot, path).string();
    };

    outputExecutable = resolve(outputExecutable);

    for (auto& sourceFile : sourceFiles) {
        sourceFile = resolve(sourceFile);
    }

    for (auto& includeDirectory : includeDirectories) {
        includeDirectory = resolve(includeDirectory);
    }
}

}