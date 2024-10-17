#pragma once
#include <iostream>
template <typename T>
void DBG(T msg, bool newline = true) {
    std::cout << msg;
    if (newline) {
        std::cout << std::endl;
    }
}
template <typename T>
void Log(T msg, bool newline = true) {
    std::cout << msg;
    if (newline) {
        std::cout << std::endl;
    }
}

