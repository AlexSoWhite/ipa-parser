//
// Created by alex on 10.11.2021.
//

#ifndef PARSER_DISPATCHER_H
#define PARSER_DISPATCHER_H


#include <string>
#include "File.h"

class Dispatcher {
    std::string temporaryDirName;
    std::string ipaName;
public:
    Dispatcher() = default;
    Dispatcher& setTemporaryDirName(const std::string & temporaryDirName);
    Dispatcher& extractIpaName(const std::string & ipaName);
    [[nodiscard]] const std::string &getTemporaryDirName() const;
    [[nodiscard]] const std::string &getIpaName() const { return this->ipaName; }
    void work(const std::string&);
    void parseMainInfoPlist();
    void parsePlist(const std::string&);
    void parseJson(const std::string&);
    void parseBinary(const std::string&);
};


#endif //PARSER_DISPATCHER_H
