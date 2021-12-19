//
// Created by alex on 13.11.2021.
//

#ifndef PARSER_PLISTPARSER_H
#define PARSER_PLISTPARSER_H


#include <string>
#include <vector>
#include <map>
#include "PlistFile.h"
#include "../pugixml-1.11/src/pugixml.hpp"
#include "PlistDate.h"

class PlistParser {
    static std::vector<PlistEntry> inflateMainDictionary(pugi::xml_node& node);
    static PlistEntry parseXml(pugi::xml_node& node);
    static std::map<std::string, PlistEntry> parseDictionary(pugi::xml_node& node);
    static std::vector<PlistEntry> parseArray(pugi::xml_node& node);
    static std::vector<char> base64Decode(const char* encodedData);
    static Date parseDate(pugi::xml_node& node);
public:
    static void readPlistFromArray(const char* byteArrayTemp, int64_t size, std::vector<PlistEntry> & message);
    PlistParser() = default;
};


#endif //PARSER_PLISTPARSER_H
