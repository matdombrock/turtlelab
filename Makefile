# Define variables
BUILD_DIR = ./build
SRC_DIR = ./src
LIB_DIR = ./lib
SDL2_STATIC_LIB_PATH_LINUX = /usr/local/lib/libSDL2.a
SDL2_STATIC_LIB_PATH_MAC = /opt/homebrew/lib/libSDL2.a
CXXFLAGS = -std=c++11
LDFLAGS_LINUX = -I/usr/local/include/SDL2 -lSDL2 -D_REENTRANT -L/usr/local/lib -lX11 -lXext -lXcursor -lXinerama -lXi -lXrandr -lXrender -lXfixes -lXxf86vm -lXdamage -lXcomposite -lXau -lXdmcp -lGL -lGLU -lGLEW -lpthread -ldl -lrt -lm
LDFLAGS_MAC = -I/opt/homebrew/include/SDL2 $(SDL2_STATIC_LIB_PATH_MAC) -framework Cocoa -framework IOKit -framework CoreVideo -framework Carbon -framework ForceFeedback -framework AudioToolbox -framework CoreAudio -framework CoreFoundation -framework CoreGraphics -framework CoreServices -framework Foundation -framework QuartzCore -framework AppKit -framework GameController -framework Metal -framework CoreHaptics
LDFLAGS_WASM = -s USE_SDL=2 -s WASM=1 -s ALLOW_MEMORY_GROWTH

# Targets
all: build_linux build_mac build_wasm

build_linux: $(BUILD_DIR)/turtle_linux

build_mac: $(BUILD_DIR)/turtle_mac

build_wasm: $(BUILD_DIR)/turtle.html

$(BUILD_DIR)/turtle_linux: $(SRC_DIR)/main.cpp $(BUILD_DIR)/s7.o $(BUILD_DIR)/duktape.o $(BUILD_DIR)/liblua.a
	mkdir -p $(BUILD_DIR)
	g++ $^ $(CXXFLAGS) $(LDFLAGS_LINUX) -o $@

$(BUILD_DIR)/turtle_mac: $(SRC_DIR)/main.cpp $(BUILD_DIR)/s7.o $(BUILD_DIR)/duktape.o $(BUILD_DIR)/liblua.a
	mkdir -p $(BUILD_DIR)
	g++ $^ $(CXXFLAGS) $(LDFLAGS_MAC) -o $@

$(BUILD_DIR)/turtle.html: $(SRC_DIR)/main.cpp $(BUILD_DIR)/liblua.wasm.a
	mkdir -p $(BUILD_DIR)
	emcc -Wl -O1 $^ $(CXXFLAGS) $(LDFLAGS_WASM) -o $@

$(BUILD_DIR)/s7.o: $(LIB_DIR)/s7/s7.c
	mkdir -p $(BUILD_DIR)
	gcc -c $< -o $@ -std=c11

$(BUILD_DIR)/duktape.o: $(LIB_DIR)/duktape/duktape.c
	mkdir -p $(BUILD_DIR)
	gcc --std=c99 -c $< -o $@

$(BUILD_DIR)/liblua.a:
	cd $(LIB_DIR)/lua && make generic
	cp $(LIB_DIR)/lua/src/liblua.a $(BUILD_DIR)

$(BUILD_DIR)/liblua.wasm.a:
	cd $(LIB_DIR)/lua/src && make clean && make CC="emcc -Os" a
	cp $(LIB_DIR)/lua/src/liblua.a $(BUILD_DIR)/liblua.wasm.a
	rm -rf $(LIB_DIR)/lua/src/*.o

clean:
	rm -rf $(BUILD_DIR)/*
	cd $(LIB_DIR)/lua && make clean

.PHONY: all build_linux build_mac build_wasm clean