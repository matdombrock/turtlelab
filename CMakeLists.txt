cmake_minimum_required(VERSION 3.10)
project(turtlelab)

set(CMAKE_CXX_STANDARD 11)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)
set(BUILD_DIR ${CMAKE_BINARY_DIR}/build)

file(MAKE_DIRECTORY ${BUILD_DIR})

add_library(s7_obj OBJECT ${LIB_DIR}/s7/s7.c)
set_target_properties(s7_obj PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_library(duktape_obj OBJECT ${LIB_DIR}/duktape/duktape.c)
set_target_properties(duktape_obj PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Build liblua.a if not present
add_custom_command(
    OUTPUT ${BUILD_DIR}/liblua.a
    COMMAND make generic
    WORKING_DIRECTORY ${LIB_DIR}/lua
    COMMAND cp ${LIB_DIR}/lua/src/liblua.a ${BUILD_DIR}/liblua.a
    DEPENDS ${LIB_DIR}/lua/Makefile
)
add_custom_target(liblua ALL DEPENDS ${BUILD_DIR}/liblua.a)

# Build liblua.wasm.a for WASM
add_custom_command(
    OUTPUT ${BUILD_DIR}/liblua.wasm.a
    COMMAND make clean
    COMMAND make CC="emcc -Os" a
    WORKING_DIRECTORY ${LIB_DIR}/lua/src
    COMMAND cp ${LIB_DIR}/lua/src/liblua.a ${BUILD_DIR}/liblua.wasm.a
    COMMAND rm -rf ${LIB_DIR}/lua/src/*.o
    DEPENDS ${LIB_DIR}/lua/Makefile
)
add_custom_target(liblua_wasm DEPENDS ${BUILD_DIR}/liblua.wasm.a)

# Linux target
if(UNIX AND NOT APPLE)
add_executable(turtle_linux ${SRC_DIR}/main.cpp $<TARGET_OBJECTS:s7_obj> $<TARGET_OBJECTS:duktape_obj>)
    add_dependencies(turtle_linux liblua)
    target_include_directories(turtle_linux PRIVATE /usr/local/include/SDL2)
    target_link_libraries(turtle_linux
        PRIVATE ${BUILD_DIR}/liblua.a
        SDL2 X11 Xext Xcursor Xinerama Xi Xrandr Xrender Xfixes Xxf86vm Xdamage Xcomposite Xau Xdmcp GL GLU GLEW pthread dl rt m
    )
    set_target_properties(turtle_linux PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})
endif()

# Mac target
if(APPLE)
add_executable(turtle_mac ${SRC_DIR}/main.cpp $<TARGET_OBJECTS:s7_obj> $<TARGET_OBJECTS:duktape_obj>)
    add_dependencies(turtle_mac liblua)
    target_include_directories(turtle_mac PRIVATE /opt/homebrew/include/SDL2)
    target_link_libraries(turtle_mac
        PRIVATE ${BUILD_DIR}/liblua.a
        "-framework Cocoa" "-framework IOKit" "-framework CoreVideo" "-framework Carbon" "-framework ForceFeedback"
        "-framework AudioToolbox" "-framework CoreAudio" "-framework CoreFoundation" "-framework CoreGraphics"
        "-framework CoreServices" "-framework Foundation" "-framework QuartzCore" "-framework AppKit"
        "-framework GameController" "-framework Metal" "-framework CoreHaptics" ${LIB_DIR}/libSDL2.a
    )
    set_target_properties(turtle_mac PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})
endif()

# WASM target
if(EMSCRIPTEN)
add_executable(turtle_wasm ${SRC_DIR}/main.cpp)
    add_dependencies(turtle_wasm liblua_wasm)
    target_link_libraries(turtle_wasm PRIVATE ${BUILD_DIR}/liblua.wasm.a)
    set_target_properties(turtle_wasm PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 -s WASM=1 -s ALLOW_MEMORY_GROWTH")
endif()

add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${BUILD_DIR}/*
    COMMAND make clean
    WORKING_DIRECTORY ${LIB_DIR}/lua
)

# Usage:
# mkdir build && cd build
# cmake ..
# make
# make clean-all
