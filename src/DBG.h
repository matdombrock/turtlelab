#pragma once
#include <iostream>
void DBG(const char *msg, bool newline = true) {
    std::cout << msg;
    if (newline) {
        std::cout << std::endl;
    }
}
void DBG(std::string msg, bool newline = true) {
    std::cout << msg;
    if (newline) {
        std::cout << std::endl;
    }
}
void DBG(int msg, bool newline = true) {
    std::cout << msg;
    if (newline) {
        std::cout << std::endl;
    }
}
void DBG(float msg, bool newline = true) {
    std::cout << msg;
    if (newline) {
        std::cout << std::endl;
    }
}