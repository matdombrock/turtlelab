#pragma once
#include <vector>
#include <cstdint>

struct Pos {
    float x;
    float y;
};

class Turtle {
public:
    Pos pos = {0, 0};
    float angle = 0;
    std::vector<Pos> hist;
    int x() {
        return round(pos.x);
    }
    int y() {
        return round(pos.y);
    }
    void forward() {
        pos.x += cos(angle);
        pos.y += sin(angle);
    }
    void rotate(uint8_t n) {
        if (n == 0) { // acts as a reset
            angle = 0;
            pos.x = round(pos.x);
            pos.y = round(pos.y);
        }
        else angle = n * 3.14159 / 128;
    }
    void rotateCW() {
        angle += 3.14159 / 128;
    }
    void rotateCCW() {
        angle -= 3.14159 / 128;
    }
    void up() {
        pos.y--;
    }
    void down() {
        pos.y++;
    }
    void left() {
        pos.x--;
    }
    void right() {
        pos.x++;
    }
    void upRight() {
        pos.x++;
        pos.y--;
    }
    void upLeft() {
        pos.x--;
        pos.y--;
    }
    void downRight() {
        pos.x++;
        pos.y++;
    }
    void downLeft() {
        pos.x--;
        pos.y++;
    }
    void teleport(uint16_t x, u_int16_t y) {
        pos.x = x;
        pos.y = y;
    } 
    void goBack(uint8_t n) {
        int index = hist.size() - n - 1;
        if (index < 0) {
            index = 0;
        }
        pos = hist[index];
    }
    void mark() {
        hist.push_back(pos);
    }
    void reset() {
        pos = {0, 0};
        angle = 0;
        hist.clear();
        mark();
    }
};