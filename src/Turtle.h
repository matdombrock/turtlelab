#pragma once
#include <vector>
#include <cstdint>

struct Pos {
    uint16_t x;
    uint16_t y;
};

class Turtle {
public:
    Pos pos = {0, 0};
    std::vector<Pos> hist;
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
    void back(uint8_t n) {
        int index = hist.size() - n - 1;
        if (index < 0) {
            index = 0;
        }
        pos = hist[index];
    }
    void mark() {
        hist.push_back(pos);
        DBG("Marking " + std::to_string(pos.x) + " " + std::to_string(pos.y));
        for (auto p : hist) {
            DBG("Hist " + std::to_string(p.x) + " " + std::to_string(p.y));
        }
    }
    void reset() {
        pos = {0, 0};
        hist.clear();
        mark();
    }
};