#pragma once
struct RunState {
    bool paused = false;
    bool mute = false;
    int ticks = 0;
};