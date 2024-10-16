#!/bin/bash
# Create build directory if it doesn't exist
mkdir -p ./build

# Path to the static SDL2 library
SDL2_STATIC_LIB_PATH="/usr/local/lib/libSDL2.a"

# Compile main.cpp for Linux
echo "Compiling main.cpp for Linux..."
g++ ./src/main.cpp \
    ./build/s7.o  \
    ./build/duktape.o \
    ./lib/lua/src/liblua.a \
    -std=c++11 \
    -I/usr/local/include/SDL2 -lSDL2 -D_REENTRANT \
    -L/usr/local/lib \
    -lX11 -lXext -lXcursor -lXinerama -lXi -lXrandr -lXrender -lXfixes -lXxf86vm -lXdamage -lXcomposite -lXau -lXdmcp -lGL -lGLU -lGLEW -lpthread -ldl -lrt -lm \
    -o ./build/turtle || exit 1

echo "Build complete."