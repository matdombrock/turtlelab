#pragma once
#include <iostream>
#include <fstream>
namespace FileIO {
    void write(std::string filename, std::string contents) {
        std::ofstream file(filename);
        file << contents;
        file.close();
    }
    
    std::string read(std::string filename) {
        std::ifstream file(filename);
        std::string str;
        std::string file_contents;
        while (std::getline(file, str))
        {
            file_contents += str;
            file_contents.push_back('\n');
        }
        return file_contents;
    }
}