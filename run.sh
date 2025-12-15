#!/bin/bash
file=${1}

mkdir -p build
gcc $(find . -name "*.c") -o build/main -lgpiod

if [ -f "build/main" ]; then
    ./build/main
fi
rm -rf build
