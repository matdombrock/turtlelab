#include <chrono>
#include <iostream>

#include "Scheme.h"
#include "JavaScript.h"
#include "Lua.h"
#include "Beep.h"
#include "Timer.h"
#include "CLIOpts.h"
#include "Log.h"
#include "FileIO.h"
#include "RunState.h"

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
    std::cout << "  -l, --lang <scm|js|lua>  Specify the lang - Overrides auto detection" << std::endl;
    std::cout << "  -d, --delay <delay>      Specify the delay between ticks" << std::endl;
    std::cout << "  -s, --skipIntro          Skip the intro" << std::endl;
    std::cout << "  -t, --hideTurtle         Hide the cursor" << std::endl;
    std::cout << "  -o, --noLoop             Do not loop the program" << std::endl;
    std::cout << "  -a, --autoClose          Automatically close the program when done" << std::endl;
    std::cout << "  -b, --noDebug            Do not show debug messages" << std::endl;
    std::cout << "  -g, --noGrid             Do not show the grid" << std::endl;
    std::cout << "  -p, --startPaused        Start the program paused" << std::endl;
    std::cout << "  -v, --volume <volume>    Set the volume" << std::endl;
    std::cout << "  -m, --mute               Mute the sound" << std::endl;
    std::cout << "  -h, --help               Display this help message" << std::endl;
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
    beep.play(false);
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
    RunState state;
    if (opts.startPaused) {
        Log("Starting paused");
        state.paused = true;
    }
    if (opts.lang == "scm") {
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
    Timer ts;
    ts.frameDelay = opts.delay;
    while (true) {
        ts.update();
        int delta = ts.delta();
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                DBG("Quitting");
                // exit(0);
                break;
            }
            // Handle window inputs
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_SPACE) {
                    state.paused = !state.paused;
                }
                if (e.key.keysym.sym == SDLK_UP) {
                    ts.frameDelay -= 0.5f * delta;
                    ts.frameDelay = std::max(ts.frameDelay, 1);
                } 
                if (e.key.keysym.sym == SDLK_DOWN) {
                    ts.frameDelay += 0.5f * delta;
                    ts.frameDelay = std::min(ts.frameDelay, 1000);
                    DBG("Delay: " + std::to_string(ts.frameDelay));
                }
                if (e.key.keysym.sym == SDLK_RIGHT) {
                    state.ticks++;
                }
                if (e.key.keysym.sym == SDLK_LEFT) {
                    state.ticks--;
                }
                if (e.key.keysym.sym == SDLK_r) {
                    state.ticks = 0;
                }
                if (e.key.keysym.sym == SDLK_m) {
                    state.mute = !state.mute;
                }
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    exit(0);
                }
            }
        }
        if (ts.frame()) {
            if (opts.lang == "scm") {
                scheme.process(state, opts);
            } 
            else if (opts.lang == "js") {
                js.process(state, opts);
            }
            else if (opts.lang == "lua") {
                lua.process(state, opts);
            }

            if (!state.paused) state.ticks++;
            // Set the window title
            std::string title = "TurtleLab - " + opts.filePath + " - " + std::to_string(ts.delta() - ts.frameDelay) + "ms";
            SDL_SetWindowTitle(win, title.c_str());
        }
    }
}

int getArgI(int index, char * argv[], int argc) {
    if (index + 1 >= argc) {
        Log("ERROR: Missing value for arg " + std::string(argv[index]));
        showHelp();
        exit(1);
    }
    return std::stoi(argv[index + 1]);
}
std::string getArgS(int index, char * argv[], int argc) {
    if (index + 1 >= argc) {
        Log("ERROR: Missing value for arg " + std::string(argv[index]));
        showHelp();
        exit(1);
    }
    return argv[index + 1];
}

std::string getMeta(std::string fileContents, std::string key) {
    if (fileContents.find(key) != std::string::npos) {
        std::string line = fileContents.substr(fileContents.find(key) + key.size());
        line = line.substr(0, line.find("\n"));
        return line;
    }
    return "";
}

int main(int argc, char* argv[]) {
    // Parse CLI args
    if (argc < 2) {
        Log("ERROR: No file path provided");
        showHelp();
        exit(1);
    }
    CLIOpts opts;
    opts.filePath = argv[1];

    std::string fileContents = FileIO::read(opts.filePath);
    if (fileContents.empty()) {
        Log("ERROR: File is empty");
        showHelp();
        exit(1);
    }
    // Parse file meta
    DBG("Meta: " + getMeta(fileContents, "NAME:"));
    std::string fileCLI = getMeta(fileContents, "CLI:");
    // Split the CLI meta into by space into args
    std::vector<std::string> fileArgvVec;
    if (!fileCLI.empty()) {
        DBG("FILE CLI: " + fileCLI);
        std::istringstream iss(fileCLI);
        fileArgvVec.push_back(argv[0]);
        fileArgvVec.push_back(opts.filePath);
        for (std::string s; iss >> s;) {
            fileArgvVec.push_back(s);
        }
        // TODO: Should prepend to existing argv
        char** argvCopy = new char*[argc];
        for (int i = 0; i < argc; i++) {
            argvCopy[i] = argv[i];
        }
        for (int i = 0; i < fileArgvVec.size(); i++) {
            argv[i] = const_cast<char*>(fileArgvVec[i].c_str());
        }
        for (int i = 2; i < argc; i++) {
            argv[i + fileArgvVec.size() - 2] = argvCopy[i];
        } 
        argc += fileArgvVec.size();
        argc -= 2;
    }
    DBG("argc: " + std::to_string(argc));
    for (int i = 0; i < argc; i++) {
        DBG("argv " + std::to_string(i)+ ": " + std::string(argv[i]));
    }

    for (int i  = 0; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-l" || arg == "--lang") {
            opts.lang = getArgS(i, argv, argc);
        }
        if (arg == "-d" || arg == "--delay") {
            DBG("Delay: " + std::to_string(getArgI(i, argv, argc)));
            opts.delay = getArgI(i, argv, argc);
        }
        if (arg == "-s" || arg == "--skipIntro") {
            opts.skipIntro = !opts.skipIntro;
        }
        if (arg == "-t" || arg == "--hideTurtle") {
            opts.hideTurtle = !opts.hideTurtle;
        }
        if (arg == "-o" || arg == "--noLoop") {
            opts.noLoop = !opts.noLoop;
        }
        if (arg == "-a" || arg == "--autoClose") {
            opts.autoClose = !opts.autoClose;
        }
        if (arg == "-b" || arg == "--noDebug") {
            opts.noDebug = !opts.noDebug;
        }
        if (arg == "-g" || arg == "--noGrid") {
            opts.noGrid = !opts.noGrid;
        }
        if (arg == "-p" || arg == "--startPaused") {
            opts.startPaused = !opts.startPaused;
        }
        if (arg == "-v" || arg == "--volume") {
            opts.volume = getArgI(i, argv, argc);
        }
        if (arg == "-m" || arg == "--mute") {
            opts.mute = !opts.mute;
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
            opts.lang = "scm";
        }
        if (opts.filePath.find(".js") != std::string::npos) {
            opts.lang = "js";
        }
        if (opts.filePath.find(".lua") != std::string::npos) {
            opts.lang = "lua";
        }
    }
    if (opts.lang == "null") {
        Log("ERROR: Cant identify file type");
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