#include "player.h"
#include <iostream>
using namespace std;

Player::Player(const string& name) : name(name) {}

void Player::play() const {
    cout << "Player " << name << " is playing." << endl;
}