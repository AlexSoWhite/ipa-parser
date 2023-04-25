//
// Created by alex on 20.11.2021.
//

#ifndef PARSER_JSONFILE_H
#define PARSER_JSONFILE_H

#include <nlohmann/json.hpp>
#include "ConfigFile.h"

class JsonFile : public ConfigFile {
    nlohmann::json data;
public:
    JsonFile() = default;
    int parse();
    JsonFile& setData(nlohmann::json & json)
    {
        this->data = json;
        return *this;
    }
    nlohmann::json & getData() { return this->data; }
    ~JsonFile() override = default;
};


#endif //PARSER_JSONFILE_H
