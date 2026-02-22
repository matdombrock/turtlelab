# TurtleLab Dependencies

This file lists all build-time and runtime dependencies required to build and run TurtleLab.

## Build Dependencies
- **CMake**: Build system generator
- **GNU Make**: For building Lua static libraries
- **GCC / G++**: C and C++ compilers
- **git**: For source management
- **wget**: For downloading tools (e.g., appimagetool)

## Runtime Dependencies
- **SDL2**: Simple DirectMedia Layer (development and runtime)
- **GLEW**: OpenGL Extension Wrangler Library
- **X11 libraries**: For graphical output on Linux
    - libXext
    - libXcursor
    - libXinerama
    - libXi
    - libXrandr
    - libXrender
    - libXfixes
    - libXxf86vm
    - libXdamage
    - libXcomposite
    - libXdmcp
- **pthread**: POSIX threads
- **dl**: Dynamic linking
- **rt**: Real-time extensions
- **GL / GLU**: OpenGL libraries

## AppImage Packaging Dependencies
- **appimagetool**: For creating AppImages
- **FUSE**: Filesystem in Userspace (libfuse)

## Optional/Platform-Specific
- **Emscripten**: For WASM builds
- **SDL2-static**: For static linking (optional)
- **MacOS Frameworks**: If building on Mac (see CMakeLists.txt)

## How to Install (Fedora Example)
```
sudo dnf install \
    cmake gcc gcc-c++ make git wget \
    SDL2-devel SDL2-static glew glew-devel \
    libXext-devel libXcursor-devel libXinerama-devel libXi-devel \
    libXrandr-devel libXrender-devel libXfixes-devel libXxf86vm-devel \
    libXdamage-devel libXcomposite-devel libXdmcp-devel \
    fuse fuse-libs
```

## How to Install (Arch Example)
```
sudo pacman -S cmake gcc make git wget \
    sdl2 sdl2_static glew libxext libxcursor libxinerama libxi \
    libxrandr libxrender libxfixes libxxf86vm libxdamage libxcomposite libxdmcp \
    fuse
```

## How to Install (Ubuntu Example)
```
sudo apt install cmake gcc g++ make git wget \
    libsdl2-dev libsdl2-static libglew-dev \
    libxext-dev libxcursor-dev libxinerama-dev libxi-dev \
    libxrandr-dev libxrender-dev libxfixes-dev libxxf86vm-dev \
    libxdamage-dev libxcomposite-dev libxdmcp-dev \
    libfuse2
```

## Notes
- For WASM builds, install Emscripten separately.
- For Mac builds, see CMakeLists.txt for required frameworks.
- Some dependencies may already be present on your system.
