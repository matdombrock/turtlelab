#!/bin/bash
# Create build directory if it doesn't exist
mkdir -p ./build

# Compile main.cpp for WebAssembly
echo "Compiling main.cpp for WebAssembly..."
emcc -Wl -O1 ./src/main.cpp \
    ./build/liblua.wasm.a \
    -std=c++11 \
    -s USE_SDL=2 \
    -s WASM=1 \
    -s ALLOW_MEMORY_GROWTH \
    -o ./build/turtle.html || exit 1

echo "Build complete."
#    ./build/duktape.wasm.o \
#./build/liblua.wasm.a \
#./build/s7.wasm.o  \