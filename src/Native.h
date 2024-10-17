#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "SDLInc.h"
#include "CLIOpts.h"
#include "DBG.h"
#include "Turtle.h"
#include "Beep.h"

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
        DBG("#" + std::to_string(index) + " ", false);
        switch  (item.command) {
            case CMD_FORWARD:
                DBG("FORWARD " + std::to_string(item.a));
                break;
            case CMD_ROTATE:
                DBG("ROTATE " + std::to_string(item.a));
                break;
            case CMD_ROTATE_CW:
                DBG("ROTATE_CW " + std::to_string(item.a));
                break;
            case CMD_ROTATE_CCW:
                DBG("ROTATE_CCW " + std::to_string(item.a));
                break;
            case CMD_UP:
                DBG("UP " + std::to_string(item.a));
                break;
            case CMD_DOWN:
                DBG("DOWN " + std::to_string(item.a));
                break;
            case CMD_LEFT:
                DBG("LEFT " + std::to_string(item.a));
                break;
            case CMD_RIGHT:
                DBG("RIGHT " + std::to_string(item.a));
                break;
            case CMD_UP_RIGHT:
                DBG("UP_RIGHT " + std::to_string(item.a));
                break;
            case CMD_UP_LEFT:
                DBG("UP_LEFT " + std::to_string(item.a));
                break;
            case CMD_DOWN_RIGHT:
                DBG("DOWN_RIGHT " + std::to_string(item.a));
                break;
            case CMD_DOWN_LEFT:
                DBG("DOWN_LEFT " + std::to_string(item.a));
                break;
            case CMD_TELEPORT:
                DBG("TELEPORT " + std::to_string(item.a) + " " + std::to_string(item.b));
                break;
            case CMD_BACK:
                DBG("GO_BACK " + std::to_string(item.a));
                break;
            case CMD_COLOR:
                DBG("COLOR " + std::to_string(item.a) + " " + std::to_string(item.b) + " " + std::to_string(item.c) + " " + std::to_string(item.d));
                break;
            case CMD_BG:
                DBG("BG " + std::to_string(item.a) + " " + std::to_string(item.b) + " " + std::to_string(item.c));
                break;
        }
    }
    void process(uint index, CLIOpts opts, SDL_Renderer *ren) {
        if (queue.size() == 0) {
            return;
        }
        if (!opts.noLoop) {
            index = index % queue.size();
        }
        else if (index >= queue.size()) {
            return;
        }
        SDL_SetRenderDrawColor(ren, bgr, bgg, bgb, 255);
        SDL_RenderClear(ren);
        QueueItem currentItem = queue[index];
        if (index == 0) {
            if (!opts.noDebug) DBG("-------");
            turtle.reset(); // Always reset on first command
        }
        turtle.reset();
        if (!opts.noDebug) printCommand(currentItem, index);
        for (int i = 0; i <= index; i++) {
            QueueItem item = queue[i];
            if (item.command == CMD_COLOR 
                || item.command == CMD_TELEPORT
                || item.command == CMD_BACK) 
            {
                handleCommand(item, ren);
                // SDL_RenderDrawPoint(ren, turtle.x, turtle.y);
                SDL_RenderDrawRect(ren, new SDL_Rect{turtle.x() * 2, turtle.y() * 2, 2, 2});
            }
            else {
                if (item.a < 0) {
                    DBG("WARN: Negative value for command #" + std::to_string(i) + " treated as absolute");
                }
                for (int i = 0; i < abs(item.a); i++) {
                    handleCommand(item, ren);
                    // SDL_RenderDrawPoint(ren, turtle.x, turtle.y);
                    SDL_RenderDrawRect(ren, new SDL_Rect{turtle.x() * 2, turtle.y() * 2, 2, 2});
                }
            }
            turtle.mark();
        }
        if (!opts.hideTurtle) {
            uint8_t r, g, b, a;
            SDL_GetRenderDrawColor(ren, &r, &g, &b, &a);
            SDL_SetRenderDrawColor(ren, 0,255,0, 255);
            SDL_RenderDrawRect(ren, new SDL_Rect{turtle.x() * 2 - 8, turtle.y() * 2 - 8, 16, 16});
            SDL_SetRenderDrawColor(ren, r, g, b, a);
        }

        SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
        SDL_RenderPresent(ren);
        // Audio
        beep.setVolume(opts.mute ? 0 : opts.volume);
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
};