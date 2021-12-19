//
// Created by alex on 10.11.2021.
//

#ifndef PARSER_UNZIPPER_H
#define PARSER_UNZIPPER_H


#include <string>

class Unzipper {
    std::string temporaryDirName;
public:
    Unzipper() = delete;
    explicit Unzipper(const std::string & name) { this->temporaryDirName = name; };
    void unzip(const std::string & directoryName);
};


#endif //PARSER_UNZIPPER_H
