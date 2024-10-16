#pragma once

class CLIOpts {
public:
    bool hideTurtle = false;
    bool noLoop = false;
    bool noDebug = false;
    bool skipIntro = false;
    uint8_t scale = 1;
    std::string lang = "null";
    std::string filePath = "null";
    uint8_t delay = 100;
    uint8_t volume = 65;
    bool mute = false;
};