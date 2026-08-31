#!/bin/bash
set -e

echo "Bootstrapping CantMake..."

SOURCES=$(find src -name "*.cpp")

g++ -std=c++23 -Iinclude $SOURCES -o cantmake -pthread

echo "CantMake built successfully as ./cantmake"
