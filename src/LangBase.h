#pragma once
#include <string>
#include "./CLIOpts.h"
#include "SDLInc.h"

class LangBase {
public:
    LangBase() {}
    ~LangBase() {}
    virtual void load(const std::string &fileContents, SDL_Renderer *renderer) {}
    virtual void run() {}
    virtual void process(int index, CLIOpts opts) {}
protected:
    std::string file;
};