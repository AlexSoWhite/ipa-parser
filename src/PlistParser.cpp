//
// Created by alex on 13.11.2021.
//

#include <fstream>
#include <iostream>
#include "../include/PlistParser.h"
#include "../pugixml-1.11/src/pugixml.hpp"
#include "../include/base64.h"
// 1 - error
// 0 - success
// maybe I'll do enum
// CFBundleExecutable

void PlistParser::readPlistFromArray(const char* byteArrayTemp, int64_t size, std::vector<PlistEntry> & content)
{
    const auto* byteArray = (const unsigned char*) byteArrayTemp;
    if (!byteArray || (size == 0))
        return;

    // using pugixml library to read xml data
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(byteArray, (size_t)size);
    if(!result)
        return;

    // there we have doc, that holds xml tree object
    // but plist is not xml, it's xml-like file
    // now we convert it to actual plist dict
    pugi::xml_node rootNode = (doc.child("plist").first_child());
    content = inflateMainDictionary(rootNode);
}

std::vector<PlistEntry>  PlistParser::inflateMainDictionary(pugi::xml_node& node) {
    // here we use a fact that plist file is always a dict
    std::string firstNodeName = node.name();
    if (firstNodeName != "dict") {
        // error
    }
    else {
        // result will be vector with one plist entry of type dict
        std::vector<PlistEntry> dict;
        dict.emplace_back(
                PlistEntry::DICT,
                parseDictionary(node)
        );
        return dict;
    }
}

PlistEntry PlistParser::parseXml(pugi::xml_node& node)
{
    std::string nodeName = node.name();

    PlistEntry result;

    switch (PlistEntry::typeMap.at(nodeName)) {
        case PlistEntry::DICT:
            result = PlistEntry(
                    PlistEntry::DICT,
                    parseDictionary(node)
            );
            break;
        case PlistEntry::ARRAY:
            result = PlistEntry(
                    PlistEntry::ARRAY,
                    parseArray(node)
            );
            break;
        case PlistEntry::STRING:
            result = PlistEntry(
                    PlistEntry::STRING,
                    std::string(node.first_child().value())
            );
            break;
        case PlistEntry::INTEGER:
            result = PlistEntry(
                    PlistEntry::INTEGER,
                    (int64_t)node.first_child().value()
            );
            break;
        case PlistEntry::REAL:
            result = PlistEntry(
                    PlistEntry::REAL,
                    node.first_child().value()
            );
            break;
        case PlistEntry::FALSE:
            result = PlistEntry(
                    PlistEntry::FALSE,
                    bool(false)
            );
            break;
        case PlistEntry::TRUE:
            result = PlistEntry(
                    PlistEntry::TRUE,
                    bool(true)
            );
            break;
        case PlistEntry::DATA:
            result = PlistEntry(
                    PlistEntry::DATA,
                    base64Decode(node.first_child().value())
            );
            break;
        case PlistEntry::DATE:
            result = PlistEntry(
                    PlistEntry::DATE,
                    parseDate(node)
            );
            break;
        default:
            break;
    }

    return result;
}

std::map<std::string, PlistEntry>  PlistParser::parseDictionary(pugi::xml_node& node)
{
    std::map<std::string, PlistEntry> _dict;

    // inflating dictionary
    for(pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
    {
        if(std::string("key") != it->name()) {
            // error
        }

        std::string _key;
        _key = it->first_child().value();
        ++it;

        if(it == node.end()) {
            //error
        }
        if(std::string("key") == it->name()) {
            //error
        }

        _dict[_key] = parseXml(*it);
    }

    return _dict;
}

std::vector<PlistEntry> PlistParser::parseArray(pugi::xml_node& node)
{
    std::vector<PlistEntry> array;
    for(auto & it : node)
        array.emplace_back(parseXml(it));
    return array;
}

std::vector<char> PlistParser::base64Decode(const char* encodedData)
{
    std::vector<char> data;
    std::insert_iterator<std::vector<char> > ii(data, data.begin());
    base64<char> b64;
    int state = 0;
    b64.get(encodedData, encodedData + strlen(encodedData), ii, state);
    return data;
}

Date PlistParser::parseDate(pugi::xml_node& node)
{
    Date date;
    date.setTimeFromXMLConvention(node.first_child().value());
    return date;
}