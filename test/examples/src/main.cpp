#include "player.h"
#include "enemy.h"
#include "world.h"

int main() {
    Player p("Hero");
    Enemy e;
    World w;

    p.play();
    e.attack();
    w.simulate();

    return 0;
}