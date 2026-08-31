#pragma once
#include <string>
using namespace std;

namespace cantmake {

class BuildSystem {
public:
    explicit BuildSystem(const string& makefileTarget);

    bool run();

private:
    string makefileTarget;
 
    bool checkFileNeedsCompilation(const string& filename, 
                                   class DependencyGraph& graph, 
                                   class HashManager& hashManager, 
                                   class CacheManager& cacheManager);
};

}