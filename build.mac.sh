#!/bin/bash
# Create build directory if it doesn't exist
mkdir -p ./build

# Path to the static SDL2 library
SDL2_STATIC_LIB_PATH="/opt/homebrew/lib/libSDL2.a"

# Compile main.cpp for native
echo "Compiling main.cpp for native..."
g++ ./src/main.cpp \
    ./build/s7.o  \
    ./build/duktape.o \
    ./lib/lua/src/liblua.a \
    -std=c++11 \
    -I/opt/homebrew/include/SDL2 $SDL2_STATIC_LIB_PATH \
    -framework Cocoa -framework IOKit -framework CoreVideo -framework Carbon -framework ForceFeedback -framework AudioToolbox -framework CoreAudio -framework CoreFoundation -framework CoreGraphics -framework CoreServices -framework Foundation -framework QuartzCore -framework AppKit -framework GameController -framework Metal -framework CoreHaptics \
    -o ./build/turtle || exit 1

echo "Build complete."