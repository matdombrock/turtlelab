#pragma once
#include <iostream>
#include "../lib/lua/src/lua.hpp"
#include "Native.h"
#include "LangBase.h"

namespace LuaBinds {
    Native native;
    SDL_Renderer *ren;
    lua_State *L;

    void init(SDL_Renderer *renderer) {
        ren = renderer;
        L = luaL_newstate();
        luaL_openlibs(L);
    }

    int print(lua_State *L) {
        int n = lua_gettop(L);
        for (int i = 1; i <= n; i++) {
            if (lua_isstring(L, i)) {
                native.print(lua_tostring(L, i));
            } else if (lua_isnumber(L, i)) {
                native.print(lua_tonumber(L, i));
            } else {
                native.print("Unknown type");
            }
        }
        std::cout << std::endl;
        return 0;
    }

    int up(lua_State *L) {
        int n = luaL_checkinteger(L, 1);
        native.up(n);
        return 0;
    }
    int down(lua_State *L) {
        int n = luaL_checkinteger(L, 1);
        native.down(n);
        return 0;
    }
    int left(lua_State *L) {
        int n = luaL_checkinteger(L, 1);
        native.left(n);
        return 0;
    }
    int right(lua_State *L) {
        int n = luaL_checkinteger(L, 1);
        native.right(n);
        return 0;
    }
    int upRight(lua_State *L) {
        int n = luaL_checkinteger(L, 1);
        native.upRight(n);
        return 0;
    }
    int upLeft(lua_State *L) {
        int n = luaL_checkinteger(L, 1);
        native.upLeft(n);
        return 0;
    }
    int downRight(lua_State *L) {
        int n = luaL_checkinteger(L, 1);
        native.downRight(n);
        return 0;
    }
    int downLeft(lua_State *L) {
        int n = luaL_checkinteger(L, 1);
        native.downLeft(n);
        return 0;
    }
    int teleport(lua_State *L) {
        int x = luaL_checkinteger(L, 1);
        int y = luaL_checkinteger(L, 2);
        native.teleport(x, y);
        return 0;
    }
    int color(lua_State *L) {
        int r = luaL_checkinteger(L, 1);
        int g = luaL_checkinteger(L, 2);
        int b = luaL_checkinteger(L, 3);
        int a = luaL_optinteger(L, 4, 255); // Default alpha value is 255 if not provided
        native.color(r, g, b, a);
        return 0;
    }
    int bg(lua_State *L) {
        int r = luaL_checkinteger(L, 1);
        int g = luaL_checkinteger(L, 2);
        int b = luaL_checkinteger(L, 3);
        native.bg(r, g, b);
        return 0;
    }

    void setBinds() {
        lua_register(L, "print", print);
        lua_register(L, "up", up);
        lua_register(L, "down", down);
        lua_register(L, "left", left);
        lua_register(L, "right", right);
        lua_register(L, "upRight", upRight);
        lua_register(L, "upLeft", upLeft);
        lua_register(L, "downRight", downRight);
        lua_register(L, "downLeft", downLeft);
        lua_register(L, "teleport", teleport);
        lua_register(L, "color", color);
        lua_register(L, "bg", bg);
    }

    std::string prependExtras(const std::string &str) {
        std::string extras = "";
        return extras + str;
    }
}

class Lua : public LangBase {
public:
    Lua() {}
    ~Lua() {
        // DBG("Closing Lua");
        // lua_close(LuaBinds::L);
    }
    void load(const std::string &fileContents, SDL_Renderer *renderer) override {
        file = fileContents;
        file = LuaBinds::prependExtras(file);
        LuaBinds::init(renderer);
        LuaBinds::setBinds();
        std::cout << "Lua initialized" << std::endl;
        
    }
    void run() override {
        std::string env = "";
        std::string fileEnv = env + file;
        if (luaL_dostring(LuaBinds::L, fileEnv.c_str())) {
            std::cerr << "Error: " << lua_tostring(LuaBinds::L, -1) << std::endl;
            exit(1);
        }
    }
    void process(int index, CLIOpts opts) override {
        LuaBinds::native.process(index, opts, LuaBinds::ren);
    }
private:
    std::string file;
};