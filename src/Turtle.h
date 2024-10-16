#pragma once
#include <cstdint>


class Turtle {
public:
    uint16_t x = 0;
    uint16_t y = 0;
    void up() {
        y--;
    }
    void down() {
        y++;
    }
    void left() {
        x--;
    }
    void right() {
        x++;
    }
    void upRight() {
        x++;
        y--;
    }
    void upLeft() {
        x--;
        y--;
    }
    void downRight() {
        x++;
        y++;
    }
    void downLeft() {
        x--;
        y++;
    }
    void teleport(uint16_t x, u_int16_t y) {
        this->x = x;
        this->y = y;
    } 
    void reset() {
        x = 0;
        y = 0;
    }
};