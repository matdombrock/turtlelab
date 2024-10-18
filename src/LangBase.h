#pragma once
#include <string>
#include "./CLIOpts.h"
#include "SDLInc.h"
#include "RunState.h"

class LangBase {
public:
    LangBase() {}
    ~LangBase() {}
    virtual void load(const std::string &fileContents, SDL_Renderer *renderer) {}
    virtual void run() {}
    virtual void process(RunState state, CLIOpts opts) {}
protected:
    std::string file;
};