//
// Created by alex on 13.11.2021.
//

#ifndef PARSER_PLISTFILE_H
#define PARSER_PLISTFILE_H


#include <vector>
#include <boost/any.hpp>
#include "ConfigFile.h"
#include "PlistEntry.h"

class PlistFile : public ConfigFile {
    std::vector<PlistEntry> data;
    void readPlist(std::istream&, std::vector<PlistEntry> &);
    void movePlistToArray(const char* byteArrayTemp, int64_t size, std::vector<PlistEntry>&);
public:
    PlistFile() = default;
    PlistFile& setData(const std::vector<PlistEntry> & data)
    {
        this->data = data;
        return *this;
    }
    int parse();
    std::vector<PlistEntry> & getData() { return this->data; }
    ~PlistFile() override = default;
    friend class PlistParser;
    friend class BinaryPlistParser;
};


#endif //PARSER_PLISTFILE_H
