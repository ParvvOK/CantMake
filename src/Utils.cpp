#include "Utils.h"
#include <algorithm>
#include <cctype>
using namespace std;

namespace cantmake {
namespace utils {

filesystem::path projectRootFromMakefile(const string& makefilePath) {
    filesystem::path makefile = filesystem::absolute(makefilePath);
    filesystem::path parent = makefile.parent_path();
    if (parent.empty()) {
        return filesystem::current_path();
    }
    return parent;
}

filesystem::path resolveProjectPath(const filesystem::path& projectRoot,
                                         const string& path) {
    filesystem::path resolved(path);
    if (resolved.is_absolute()) {
        return resolved;
    }
    return projectRoot / resolved;
}

string trim(const string& str) {
    auto start = str.begin();
    while (start != str.end() && isspace(static_cast<unsigned char>(*start))) {
        start++;
    }

    auto end = str.end();
    do {
        end--;
    } while (distance(start, end) > 0 && isspace(static_cast<unsigned char>(*end)));

    return string(start, end + 1);
}

}
}