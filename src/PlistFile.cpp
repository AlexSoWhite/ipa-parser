//
// Created by alex on 13.11.2021.
//

#include <iostream>
#include <fstream>
#include "../include/PlistFile.h"
#include "../include/PlistParser.h"
#include "../include/BinaryPlistParser.h"

//TODO error catching
int PlistFile::parse() {
    // opening plist file
    std::ifstream fin;
    fin.open(this->path);
    if(!fin.is_open()) {
        return 1;
    }

    // initializing plist dict
    std::vector<PlistEntry> dict;

    // actual parsing
    readPlist(fin, dict);

    // saving the result
    this->data = dict;
    fin.close();
    std::ofstream fout(this->path);
    if (fout.is_open()) {
        for (const auto &e: this->data) {
            fout << e;
        }
    }
    return 0;
}

void PlistFile::readPlist(std::istream& stream, std::vector<PlistEntry> & message) {
    int start = stream.tellg();
    stream.seekg(0, std::ifstream::end);
    int size = ((int) stream.tellg()) - start;
    if(size > 0)
    {
        stream.seekg(0, std::ifstream::beg);
        std::vector<char> buffer(size);
        stream.read( (char *)&buffer[0], size );

        movePlistToArray(&buffer[0], size, message);
    }
    else
    {
        // error
    }
}

void PlistFile::movePlistToArray(
    const char *byteArrayTemp,
    int64_t size,
    std::vector<PlistEntry> & message
) {
    const auto* byteArray = (const unsigned char*) byteArrayTemp;
    if (!byteArray || (size == 0))
        return;

    std::string magicHeader((const char*) byteArray, 8);
    if(magicHeader == "bplist00") {
        BinaryPlistParser::readBinaryPlistFromArray(
            byteArrayTemp,
            size,
            message
        );
    }
    else {
        PlistParser::readPlistFromArray(
            byteArrayTemp,
            size,
            message
        );
    }
}
