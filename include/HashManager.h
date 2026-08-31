#pragma once
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

namespace cantmake {

class HashManager {
public:
    string computeHash(const string& filename);

    const unordered_map<string, string>& getComputedHashes() const;

private:
    string calculateFileHash(const string& filename) const;

    unordered_map<string, string> computedHashes;
};

}