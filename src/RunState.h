#pragma once
#include <string>
struct RunState {
    bool paused = false;
    bool mute = false;
    int ticks = 0;
    std::string fileName = "";
};