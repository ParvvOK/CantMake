#include "HashManager.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdint>
using namespace std;

namespace cantmake {

string HashManager::computeHash(const string& filename) {
    if (computedHashes.find(filename) != computedHashes.end()) {
        return computedHashes[filename];
    }
    
    string hashStr = calculateFileHash(filename);
    if (!hashStr.empty()) {
        computedHashes[filename] = hashStr;
    }
    return hashStr;
}

const unordered_map<string, string>& HashManager::getComputedHashes() const {
    return computedHashes;
}

string HashManager::calculateFileHash(const string& filename) const {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        return "";
    }

    const uint64_t fnv_prime = 1099511628211ull;
    uint64_t hash = 14695981039346656037ull;

    char buffer[4096];
    while (file.read(buffer, sizeof(buffer))) {
        streamsize bytes = file.gcount();
        for (streamsize i = 0; i < bytes; ++i) {
            hash ^= static_cast<unsigned char>(buffer[i]);
            hash *= fnv_prime;
        }
    }
    
    streamsize bytes = file.gcount();
    for (streamsize i = 0; i < bytes; ++i) {
        hash ^= static_cast<unsigned char>(buffer[i]);
        hash *= fnv_prime;
    }

    stringstream ss;
    ss << hex << setfill('0') << setw(16) << hash;
    return ss.str();
}

}