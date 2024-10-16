#pragma once
#include <chrono>

class Timer {
public: 
    int frameDelay;
    Timer() {
        start = std::chrono::high_resolution_clock::now();
        last = start;
        frameDelay = 1000;
        update();
    }
    ~Timer() {}
    void update() {
        tmp = std::chrono::high_resolution_clock::now();
        diff = std::chrono::duration_cast<std::chrono::milliseconds>(tmp - last);
        if (diff.count() > frameDelay) {
            newFrame = true;
            // DBG("Frame");
        }
    }
    bool frame() {
        if (newFrame) {
            newFrame = false;
            last = tmp;
            return true;
        }
        return false;
    }
    int delta() {
        return diff.count();
    }
    // Returns total time elapsed since start
    int elapsed() {
       return std::chrono::duration_cast<std::chrono::milliseconds>(tmp - start).count(); 
    }
private:
    bool newFrame = false;
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::time_point<std::chrono::high_resolution_clock> last;
    std::chrono::time_point<std::chrono::high_resolution_clock> tmp;
    std::chrono::milliseconds diff;
};