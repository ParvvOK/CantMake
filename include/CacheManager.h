#pragma once
#include <string>
#include <unordered_map>
using namespace std;

namespace cantmake {

class CacheManager {
public:
    explicit CacheManager(const string& cacheFilePath);

    void load();
    void save(const unordered_map<string, string>& hashes);

    string getCachedHash(const string& filename) const;
    bool hasChanged(const string& filename, const string& currentHash) const;

private:
    string cacheFilePath;
    unordered_map<string, string> cachedHashes;
};

}