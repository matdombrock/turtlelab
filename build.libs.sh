#!/bin/bash
mkdir -p ./build

echo "Compiling s7..."
gcc -c ./lib/s7/s7.c -o ./build/s7.o -std=c11 || exit 1

echo "Compiling Duktape..."
gcc --std=c99 -c ./lib/duktape/duktape.c -o ./build/duktape.o || exit 1

echo "Compiling Lua..."
cd ./lib/lua 
make generic || exit 1
cp src/liblua.a ../../build || exit 1
rm -rf src/*.o

echo "Wrote build artifacts to ./build"