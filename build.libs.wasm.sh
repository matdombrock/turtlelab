#!/bin/bash
mkdir -p ./build

echo "Compiling s7..."
emcc -Os -c ./lib/s7/s7.c -o ./build/s7.wasm.o -std=c11 || exit 1

echo "Compiling Duktape..."
emcc -Os --std=c99 -c ./lib/duktape/duktape.c -o ./build/duktape.wasm.o || exit 1

echo "Compiling Lua..."
cd ./lib/lua/src
make clean
make CC="emcc -Os" a || exit 1
cp liblua.a ../../../build/liblua.wasm.a || exit 1
rm -rf src/*.o

echo "Wrote build artifacts to ./build"