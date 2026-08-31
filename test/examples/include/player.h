#pragma once
#include <string>
using namespace std;

class Player {
public:
    Player(const string& name);
    void play() const;
private:
    string name;
};