#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "SDLInc.h"
#include "CLIOpts.h"
#include "Log.h"
#include "Turtle.h"
#include "Beep.h"
#include "RunState.h"

enum Command {
    CMD_FORWARD,
    CMD_ROTATE,
    CMD_ROTATE_CW,
    CMD_ROTATE_CCW,
    CMD_UP,
    CMD_DOWN,
    CMD_LEFT,
    CMD_RIGHT,
    CMD_UP_RIGHT,
    CMD_UP_LEFT,
    CMD_DOWN_RIGHT,
    CMD_DOWN_LEFT,
    CMD_TELEPORT,
    CMD_BACK,
    CMD_COLOR,
    CMD_BG
};

class QueueItem {
public:
    Command command;
    int a;
    int b;
    int c;
    int d;
    QueueItem(Command command, int a = 1, int b = 0, int c = 0, int d = 0) {
        this->command = command;
        this->a = a;
        this->b = b;
        this->c = c;
        this->d = d;
    }
};

class Native {
public:
    Turtle turtle;
    Beep beep;
    std::vector<QueueItem> queue;
    uint bgr = 15;
    uint bgg = 15;
    uint bgb = 15;
    Native() {}
    ~Native() {}
    template <typename T>
    void print(T value) {
        std::cout << value << std::endl;
    }
    void color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
        queue.push_back(QueueItem(CMD_COLOR, r, g, b, a));
    }
    void bg(uint8_t r, uint8_t g, uint8_t b) {
        queue.push_back(QueueItem(CMD_BG, r, g, b));
    }
    void forward(uint n) {
        queue.push_back(QueueItem(CMD_FORWARD, n));
    }
    void rotate(uint8_t n) {
        queue.push_back(QueueItem(CMD_ROTATE, n));
    }
    void rotateCW(float a) {
        queue.push_back(QueueItem(CMD_ROTATE_CW, a));
    }
    void rotateCCW(float a) {
        queue.push_back(QueueItem(CMD_ROTATE_CCW, a));
    }
    void up(uint8_t n) {    
        queue.push_back(QueueItem(CMD_UP, n));
    }
    void down(uint8_t n) {
        queue.push_back(QueueItem(CMD_DOWN, n));
    }
    void left(uint8_t n) {
        queue.push_back(QueueItem(CMD_LEFT, n));
    }
    void right(uint8_t n) {
        queue.push_back(QueueItem(CMD_RIGHT, n));
    }
    void upRight(uint8_t n) {
        queue.push_back(QueueItem(CMD_UP_RIGHT, n));
    }
    void upLeft(uint8_t n) {
        queue.push_back(QueueItem(CMD_UP_LEFT, n));
    }
    void downRight(uint8_t n) {
        queue.push_back(QueueItem(CMD_DOWN_RIGHT, n));
    }
    void downLeft(uint8_t n) {
        queue.push_back(QueueItem(CMD_DOWN_LEFT, n));
    }
    void teleport(uint8_t x, uint y) {
        queue.push_back(QueueItem(CMD_TELEPORT, x, y));
    }
    void goBack(uint8_t n) {
        queue.push_back(QueueItem(CMD_BACK, n));
    }
    void process(RunState state, CLIOpts opts, SDL_Renderer *ren) {
        if (queue.size() == 0) {
            return;
        }
        if (!opts.noLoop && !opts.autoClose) {
            state.ticks = state.ticks % queue.size();
        }
        else if (state.ticks >= queue.size()) {
            if (opts.autoClose) {
                DBG("Auto closing");
                exit(0);
            }
            
            return;
        }

        const int top = 36;

        SDL_SetRenderDrawColor(ren, bgr, bgg, bgb, 255);
        SDL_RenderClear(ren);

        // Draw a grid
        if (!opts.noGrid) {
            SDL_SetRenderDrawColor(ren, 48, 48, 48, 255);
            for (int x = 0; x < 512; x+=8) {
                for (int y = 0; y < 512; y+=8) {
                    SDL_RenderDrawLine(ren, x, top, x, 512 + top);
                    SDL_RenderDrawLine(ren, 0, y + top, 512, y + top);
                }
            }
            SDL_SetRenderDrawColor(ren, 64,64,64, 255);
            SDL_RenderDrawLine(ren, 0, 256 + top, 512, 256 + top);
            SDL_RenderDrawLine(ren, 256, 0 + top, 256, 512 + top);
        }

        QueueItem currentItem = queue[state.ticks];
        if (state.ticks == 0) {
            if (!opts.noDebug) Log("-------");
            turtle.reset(); // Always reset on first command
        }
        turtle.reset();
        if (!opts.noDebug && !state.paused) printCommand(currentItem, state.ticks);
        for (int i = 0; i <= state.ticks; i++) {
            QueueItem item = queue[i];
            if (item.command == CMD_COLOR 
                || item.command == CMD_TELEPORT
                || item.command == CMD_BACK) 
            {
                handleCommand(item, ren);
                // SDL_RenderDrawPoint(ren, turtle.x, turtle.y);
                SDL_RenderDrawRect(ren, new SDL_Rect{turtle.x() * 2, turtle.y() * 2 + top, 2, 2});
            }
            else {
                if (item.a < 0) {
                    Log("WARN: Negative value for command #" + std::to_string(i) + " treated as absolute");
                }
                // Special case for rotate 0
                if (item.command == CMD_ROTATE && item.a == 0) {
                    handleCommand(item, ren);
                    SDL_RenderDrawRect(ren, new SDL_Rect{turtle.x() * 2, turtle.y() * 2 + top, 2, 2});
                }
                else {
                    for (int i = 0; i < abs(item.a); i++) {
                        handleCommand(item, ren);
                        // SDL_RenderDrawPoint(ren, turtle.x, turtle.y);
                        SDL_RenderDrawRect(ren, new SDL_Rect{turtle.x() * 2, turtle.y() * 2 + top, 2, 2});
                    }
                }
            }
            turtle.mark();
        }
        if (!opts.hideTurtle) {
            uint8_t r, g, b, a;
            SDL_GetRenderDrawColor(ren, &r, &g, &b, &a);
            SDL_SetRenderDrawColor(ren, 0,255,0, 255);
            // SDL_RenderDrawRect(ren, new SDL_Rect{turtle.x() * 2 - 8, turtle.y() * 2 - 8, 16, 16});
            int curSize = 16;
            int x = turtle.x() * 2;
            int y = turtle.y() * 2 + top;
            // SDL_RenderFillRect(ren, new SDL_Rect{x - 8, y - 8, 16, 16});
            SDL_RenderDrawLine(ren, 
                x, 
                y,
                x + curSize * -cos(turtle.angle - M_PI / 6), 
                y + curSize * -sin(turtle.angle - M_PI / 6)
            );
            SDL_RenderDrawLine(ren, 
                x, 
                y, 
                x + curSize * -cos(turtle.angle + M_PI / 6), 
                y + curSize * -sin(turtle.angle + M_PI / 6)
            );
            SDL_RenderDrawLine(ren, 
                x + curSize * -cos(turtle.angle - M_PI / 6), 
                y + curSize * -sin(turtle.angle - M_PI / 6), 
                x + curSize * -cos(turtle.angle + M_PI / 6), 
                y + curSize * -sin(turtle.angle + M_PI / 6)
            );
            SDL_SetRenderDrawColor(ren, r, g, b, a);
        }

        if (state.paused) {
            SDL_SetRenderDrawColor(ren, 196, 128, 0, 255);
            SDL_RenderFillRect(ren, new SDL_Rect{0, top, 512, 4});
            SDL_RenderFillRect(ren, new SDL_Rect{0, 508 + top, 512, 4});
            SDL_RenderFillRect(ren, new SDL_Rect{0, top, 4, 512});
            SDL_RenderFillRect(ren, new SDL_Rect{508, top, 4, 512});
        }

        // Draw top bar
        SDL_SetRenderDrawColor(ren, 33, 33, 33, 200);
        SDL_RenderFillRect(ren, new SDL_Rect{0, 0, 512, top});

        // T
        (state.ticks % 10 != 0) ? SDL_SetRenderDrawColor(ren, 128, 128, 128, 200) : SDL_SetRenderDrawColor(ren, 138, 138, 136, 216);
        SDL_RenderFillRect(ren, new SDL_Rect{4, 4, 32, 8});
        SDL_RenderFillRect(ren, new SDL_Rect{16, 4, 8, 28});
        // U
        (state.ticks % 10 != 1) ? SDL_SetRenderDrawColor(ren, 128, 128, 128, 200) : SDL_SetRenderDrawColor(ren, 138, 138, 136, 216);
        SDL_RenderFillRect(ren, new SDL_Rect{48, 4, 8, 28}); 
        SDL_RenderFillRect(ren, new SDL_Rect{48, 24, 24, 8});
        SDL_RenderFillRect(ren, new SDL_Rect{64, 4, 8, 28});
        // R
        (state.ticks % 10 != 2) ? SDL_SetRenderDrawColor(ren, 128, 128, 128, 200) : SDL_SetRenderDrawColor(ren, 138, 138, 136, 216);
        SDL_RenderFillRect(ren, new SDL_Rect{88, 4, 8, 28});
        SDL_RenderFillRect(ren, new SDL_Rect{88, 4, 24, 8});
        SDL_RenderFillRect(ren, new SDL_Rect{112, 4, 8, 16});
        SDL_RenderFillRect(ren, new SDL_Rect{96, 16, 16, 8});
        SDL_RenderFillRect(ren, new SDL_Rect{112, 24, 8, 8});
        // T
        (state.ticks % 10 != 3) ? SDL_SetRenderDrawColor(ren, 128, 128, 128, 200) : SDL_SetRenderDrawColor(ren, 138, 138, 136, 216);
        SDL_RenderFillRect(ren, new SDL_Rect{128, 4, 32, 8});
        SDL_RenderFillRect(ren, new SDL_Rect{140, 4, 8, 28});
        // L
        (state.ticks % 10 != 4) ? SDL_SetRenderDrawColor(ren, 128, 128, 128, 200) : SDL_SetRenderDrawColor(ren, 138, 138, 136, 216);
        SDL_RenderFillRect(ren, new SDL_Rect{172, 4, 8, 28});
        SDL_RenderFillRect(ren, new SDL_Rect{172, 24, 24, 8});
        // E
        (state.ticks % 10 != 5) ? SDL_SetRenderDrawColor(ren, 128, 128, 128, 200) : SDL_SetRenderDrawColor(ren, 138, 138, 136, 216);
        SDL_RenderFillRect(ren, new SDL_Rect{212, 4, 8, 28});
        SDL_RenderFillRect(ren, new SDL_Rect{212, 4, 24, 8});
        SDL_RenderFillRect(ren, new SDL_Rect{212, 16, 16, 8});
        SDL_RenderFillRect(ren, new SDL_Rect{212, 24, 24, 8});
        // L
        (state.ticks % 10 != 6) ? SDL_SetRenderDrawColor(ren, 128, 128, 128, 200) : SDL_SetRenderDrawColor(ren, 138, 138, 136, 216);
        SDL_RenderFillRect(ren, new SDL_Rect{248, 4, 8, 28});
        SDL_RenderFillRect(ren, new SDL_Rect{248, 24, 24, 8});
        // A
        (state.ticks % 10 != 7) ? SDL_SetRenderDrawColor(ren, 128, 128, 128, 200) : SDL_SetRenderDrawColor(ren, 138, 138, 136, 216);
        SDL_RenderFillRect(ren, new SDL_Rect{284, 4, 8, 28});
        SDL_RenderFillRect(ren, new SDL_Rect{284, 4, 24, 8});
        SDL_RenderFillRect(ren, new SDL_Rect{284, 16, 24, 8});
        SDL_RenderFillRect(ren, new SDL_Rect{308, 4, 8, 28});
        // B
        (state.ticks % 10 != 8) ? SDL_SetRenderDrawColor(ren, 128, 128, 128, 200) : SDL_SetRenderDrawColor(ren, 138, 138, 136, 216);
        SDL_RenderFillRect(ren, new SDL_Rect{324, 4, 8, 28});
        SDL_RenderFillRect(ren, new SDL_Rect{324, 4, 24, 8});
        SDL_RenderFillRect(ren, new SDL_Rect{324, 16, 24, 8});
        SDL_RenderFillRect(ren, new SDL_Rect{324, 24, 24, 8});
        SDL_RenderFillRect(ren, new SDL_Rect{348, 4, 8, 16});
        SDL_RenderFillRect(ren, new SDL_Rect{348, 16, 8, 8});
        // Draw a turtle after the title
        (state.ticks % 10 != 9) ? SDL_SetRenderDrawColor(ren, 0, 255, 0, 128) : SDL_SetRenderDrawColor(ren, 0, 255, 0, 148);
        SDL_RenderFillRect(ren, new SDL_Rect{404, 4, 24, 4}); 
        SDL_RenderFillRect(ren, new SDL_Rect{400, 8, 32, 16});
        (state.ticks % 10 != 9) ? SDL_SetRenderDrawColor(ren, 255, 255, 255, 128) : SDL_SetRenderDrawColor(ren, 255, 255, 255, 148); 
        SDL_RenderFillRect(ren, new SDL_Rect{400, 20, 8, 8}); 
        SDL_RenderFillRect(ren, new SDL_Rect{420, 20, 8, 8}); 
        SDL_RenderFillRect(ren, new SDL_Rect{432, 12, 12, 12}); 

        SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
        SDL_RenderPresent(ren);
        // Audio
        beep.setVolume(opts.mute || state.mute ? 0 : opts.volume);
        beep.play();
        beep.freq = 220;
        beep.freq += turtle.x() * 10;
        beep.freq += turtle.y() * 20;
        uint8_t r,g,b,a;
        SDL_GetRenderDrawColor(ren, &r, &g, &b, &a);
        beep.m1 = r / 255.0f;
        beep.m2 = g / 255.0f;
        beep.m3 = b / 255.0f;
    }
private:
    void handleCommand(QueueItem item, SDL_Renderer *ren) {
        switch (item.command ) {
            case CMD_FORWARD:
                turtle.forward();
                break;
            case CMD_ROTATE:
                turtle.rotate(item.a);
                break;
            case CMD_ROTATE_CW:
                turtle.rotateCW();
                break;
            case CMD_ROTATE_CCW:
                turtle.rotateCCW();
                break;
            case CMD_UP:
                turtle.up();
                break;
            case CMD_DOWN:
                turtle.down();
                break;
            case CMD_LEFT:
                turtle.left();
                break;
            case CMD_RIGHT:
                turtle.right();
                break;
            case CMD_UP_RIGHT:
                turtle.upRight();
                break;
            case CMD_UP_LEFT:
                turtle.upLeft();
                break;
            case CMD_DOWN_RIGHT:
                turtle.downRight();
                break;
            case CMD_DOWN_LEFT:
                turtle.downLeft();
                break;
            case CMD_TELEPORT:
                turtle.teleport(item.a, item.b);
                break;
            case CMD_BACK:
                turtle.goBack(item.a);
                break;
            case CMD_COLOR:
                SDL_SetRenderDrawColor(ren, item.a, item.b, item.c, item.d);
                break;
            case CMD_BG:
                bgr = item.a;
                bgg = item.b;
                bgb = item.c;
                break;   
        }
    }
    void printCommand(QueueItem item, uint index) {
        Log("#" + std::to_string(index) + " ", false);
        switch  (item.command) {
            case CMD_FORWARD:
                Log("FORWARD " + std::to_string(item.a));
                break;
            case CMD_ROTATE:
                Log("ROTATE " + std::to_string(item.a));
                break;
            case CMD_ROTATE_CW:
                Log("ROTATE_CW " + std::to_string(item.a));
                break;
            case CMD_ROTATE_CCW:
                Log("ROTATE_CCW " + std::to_string(item.a));
                break;
            case CMD_UP:
                Log("UP " + std::to_string(item.a));
                break;
            case CMD_DOWN:
                Log("DOWN " + std::to_string(item.a));
                break;
            case CMD_LEFT:
                Log("LEFT " + std::to_string(item.a));
                break;
            case CMD_RIGHT:
                Log("RIGHT " + std::to_string(item.a));
                break;
            case CMD_UP_RIGHT:
                Log("UP_RIGHT " + std::to_string(item.a));
                break;
            case CMD_UP_LEFT:
                Log("UP_LEFT " + std::to_string(item.a));
                break;
            case CMD_DOWN_RIGHT:
                Log("DOWN_RIGHT " + std::to_string(item.a));
                break;
            case CMD_DOWN_LEFT:
                Log("DOWN_LEFT " + std::to_string(item.a));
                break;
            case CMD_TELEPORT:
                Log("TELEPORT " + std::to_string(item.a) + " " + std::to_string(item.b));
                break;
            case CMD_BACK:
                Log("GO_BACK " + std::to_string(item.a));
                break;
            case CMD_COLOR:
                Log("COLOR " + std::to_string(item.a) + " " + std::to_string(item.b) + " " + std::to_string(item.c) + " " + std::to_string(item.d));
                break;
            case CMD_BG:
                Log("BG " + std::to_string(item.a) + " " + std::to_string(item.b) + " " + std::to_string(item.c));
                break;
        }
    }
};