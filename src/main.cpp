#include "BuildSystem.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    string target = "makefile.cant";
    if (argc > 1) {
        target = argv[1];
    }

    cantmake::BuildSystem buildSystem(target);
    
    if (buildSystem.run()) {
        return 0;
    } else {
        return 1;
    }
}