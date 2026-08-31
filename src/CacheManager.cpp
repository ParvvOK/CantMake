#include "CacheManager.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
using namespace std;

namespace cantmake {

CacheManager::CacheManager(const string& cacheFilePath)
    : cacheFilePath(cacheFilePath) {}

void CacheManager::load() {
    cachedHashes.clear();
    ifstream file(cacheFilePath);
    if (!file.is_open()) {
        return;
    }

    string filename;
    string hash;
    
    while (getline(file, filename) && getline(file, hash)) {
        filename = utils::trim(filename);
        hash = utils::trim(hash);
        
        if (!filename.empty() && !hash.empty()) {
            cachedHashes[filename] = hash;
        }
    }
}

void CacheManager::save(const unordered_map<string, string>& hashes) {
    ofstream file(cacheFilePath);
    if (!file.is_open()) {
        cerr << "Warning: Could not open cache file for writing: " << cacheFilePath << endl;
        return;
    }

    for (const auto& [filename, hash] : hashes) {
        file << filename << "\n" << hash << "\n";
    }
}

string CacheManager::getCachedHash(const string& filename) const {
    auto it = cachedHashes.find(filename);
    if (it != cachedHashes.end()) {
        return it->second;
    }
    return "";
}

bool CacheManager::hasChanged(const string& filename, const string& currentHash) const {
    return getCachedHash(filename) != currentHash;
}
}