#include <chrono>
#include <iostream>

#include "Scheme.h"
#include "JavaScript.h"
#include "Lua.h"
#include "Beep.h"
#include "Timer.h"
#include "CLIOpts.h"
#include "DBG.h"
#include "FileIO.h"

Scheme scheme;
JavaScript js;
Lua lua;

SDL_Window *win = nullptr;
SDL_Renderer* ren;

void showHelp(bool full = false) {
    if (!full) {
        std::cout << "Usage: turtle <file> [options]" << std::endl;
        std::cout << "--help        show help" << std::endl;
        return;
    }
    std::cout << "-------" << std::endl;
    std::cout << "Usage: turtle <file> [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -l, --lang <lang>     Specify the lang (scheme, js) - Overrides auto detection" << std::endl;
    std::cout << "  -d, --delay <delay>   Specify the delay between ticks" << std::endl;
    std::cout << "  -s, --skipIntro       Skip the intro" << std::endl;
    std::cout << "  -t, --hideTurtle      Hide the cursor" << std::endl;
    std::cout << "  -o, --noLoop          Do not loop the program" << std::endl;
    std::cout << "  -b, --noDebug         Do not show debug messages" << std::endl;
    std::cout << "  -v, --volume <volume> Set the volume" << std::endl;
    std::cout << "  -m, --mute            Mute the sound" << std::endl;
    std::cout << "  -h, --help            Display this help message" << std::endl;
}

void init(CLIOpts opts) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    std::string title = "TurtleLab - " + opts.filePath;
    win = SDL_CreateWindow(title.c_str(), 100, 100, 512 * opts.scale, 512 * opts.scale, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (win == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr) {
        SDL_DestroyWindow(win);
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        exit(1);
    }

    SDL_RenderSetScale(ren, opts.scale, opts.scale);
    std::cout << "SDL initialized" << std::endl;
}

void intro(CLIOpts opts) {
    int ticks = 0;
    Timer ts;
    ts.frameDelay = 50;
    Beep beep;
    beep.setVolume(opts.mute ? 0 : opts.volume);
    beep.play();
    while (true) {
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                break;
            }
        }
        ts.update();
        if (ts.frame()) {
            SDL_SetRenderDrawColor(ren, ticks * 64, 55,0, 255);
            SDL_RenderClear(ren);
            SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
            SDL_RenderFillRect(ren, new SDL_Rect{32, 32, 64, 32});
            SDL_RenderFillRect(ren, new SDL_Rect{48, 64, 32, 64});
            SDL_RenderFillRect(ren, new SDL_Rect{128, 96, 64, 32});
            SDL_RenderFillRect(ren, new SDL_Rect{128, 32, 32, 64});
            // SDL_RenderFillRect(ren, new SDL_Rect{128, 128, 64, 32});
            SDL_RenderPresent(ren);
            ticks++;   
            beep.freq *= 2;
        }
        if (ticks > 6) {
            break;
        }
    }
    beep.stop();
}

void run(CLIOpts opts, std::string fileContents = "") { 
    if (opts.lang == "scheme") {
        scheme.load(fileContents, ren);
        scheme.run();
    } 
    else if (opts.lang == "js") {  
        js.load(fileContents, ren);
        js.run();
    }
    else if (opts.lang == "lua") {
        lua.load(fileContents, ren);
        lua.run();
    }
    int ticks = 0;
    int delayTime = opts.delay;
    Timer ts;
    ts.frameDelay = delayTime;
    while (true) {
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                DBG("Quitting");
                // exit(0);
                break;
            }
        }
        ts.update();
        if (ts.frame()) {
            int x = 0;
            int y = 0;
            if (opts.lang == "scheme") {
                scheme.process(ticks, opts);
                x = SchemeBinds::native.turtle.x;
                y = SchemeBinds::native.turtle.y;
            } 
            else if (opts.lang == "js") {
                js.process(ticks, opts);
                x = JavaScriptBinds::native.turtle.x;
                y = JavaScriptBinds::native.turtle.y;
            }
            else if (opts.lang == "lua") {
                lua.process(ticks, opts);
                x = LuaBinds::native.turtle.x;
                y = LuaBinds::native.turtle.y;
            }
            // DBG("Freq: " + std::to_string(beep.freq));
            // beep.freq = ticks % 2 == 0 ? 220 : 440;
            ticks++;
            // Set the window title
            std::string title = "TurtleLab - " + opts.filePath + " - " + std::to_string(ts.delta() - opts.delay) + "ms";
            SDL_SetWindowTitle(win, title.c_str());
        }
    }
}

int getArgI(int index, char * argv[], int argc) {
    if (index + 1 >= argc) {
        DBG("ERROR: Missing value for arg " + std::string(argv[index]));
        showHelp();
        exit(1);
    }
    return std::stoi(argv[index + 1]);
}
std::string getArgS(int index, char * argv[], int argc) {
    if (index + 1 >= argc) {
        DBG("ERROR: Missing value for arg " + std::string(argv[index]));
        showHelp();
        exit(1);
    }
    return argv[index + 1];
}

int main(int argc, char* argv[]) {
    // Parse CLI args
    if (argc < 2) {
        DBG("ERROR: No file path provided");
        showHelp();
        exit(1);
    }
    CLIOpts opts;
    opts.filePath = argv[1];
    for (int i  = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-l" || arg == "--lang") {
            opts.lang = getArgS(i, argv, argc);
        }
        if (arg == "-d" || arg == "--delay") {
            opts.delay = getArgI(i, argv, argc);
        }
        if (arg == "-s" || arg == "--skipIntro") {
            opts.skipIntro = true;
        }
        if (arg == "-t" || arg == "--hideTurtle") {
            opts.hideTurtle = true;
        }
        if (arg == "-o" || arg == "--noLoop") {
            opts.noLoop = true;
        }
        if (arg == "-b" || arg == "--noDebug") {
            opts.noDebug = true;
        }
        if (arg == "-v" || arg == "--volume") {
            opts.volume = getArgI(i, argv, argc);
        }
        if (arg == "-m" || arg == "--mute") {
            opts.mute = true;
        }
        if (arg == "-x" || arg == "--scale") {
            opts.scale = getArgI(i, argv, argc);
        }
        if (arg == "-h" || arg == "--help") {
            showHelp(true);
            exit(0);
        }
    }
    // If lang is not provided, try to infer it from the file extension
    if (opts.lang == "null") {
        if (opts.filePath.find(".scm") != std::string::npos) {
            opts.lang = "scheme";
        }
        if (opts.filePath.find(".js") != std::string::npos) {
            opts.lang = "js";
        }
        if (opts.filePath.find(".lua") != std::string::npos) {
            opts.lang = "lua";
        }
    }
    if (opts.lang == "null") {
        DBG("ERROR: Cant identify file type");
        showHelp();
        exit(1);
    }
    std::string fileContents = FileIO::read(opts.filePath);
    if (fileContents.empty()) {
        DBG("ERROR: File is empty");
        showHelp();
        exit(1);
    }

    init(opts);
    if (!opts.skipIntro) intro(opts);
    run(opts, fileContents);

    // Clean up resources when done
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    DBG("Final Exit");
    return 0;
}