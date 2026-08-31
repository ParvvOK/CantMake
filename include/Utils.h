#pragma once
#include <filesystem>
#include <string>
using namespace std;

namespace cantmake {
namespace utils {

string trim(const string& str);

filesystem::path projectRootFromMakefile(const string& makefilePath);
filesystem::path resolveProjectPath(const filesystem::path& projectRoot,
                                         const string& path);

  }
}