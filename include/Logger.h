//
// Created by nafanya on 4/20/23.
//

#ifndef PARSER_LOGGER_H
#define PARSER_LOGGER_H

#include <string>
#include <fstream>
#include <iostream>

class Logger {
    std::ofstream file;
public:
    explicit Logger(const std::string& path) {
        this->file = std::ofstream(path);

    }
    void log(const std::string& message) {
        std::cout<<message<<std::endl;
        file << message << std::endl;
    }
};

#endif //PARSER_LOGGER_H
