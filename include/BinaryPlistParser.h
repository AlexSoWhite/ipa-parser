//
// Created by alex on 14.11.2021.
//

#ifndef PARSER_BINARYPLISTPARSER_H
#define PARSER_BINARYPLISTPARSER_H


#include <vector>
#include <string>
#include "PlistEntry.h"
#include "PlistFile.h"

struct PlistHelperData
{
    // binary helper data
    std::vector<int32_t> _offsetTable;
    std::vector<unsigned char> _objectTable;
    int32_t _offsetByteSize;
    int64_t _offsetTableOffset;

    int32_t _objRefSize;
    int32_t _refCount;
};

class BinaryPlistParser {

    template <typename IntegerType>
    static    IntegerType bytesToInt(const unsigned char* bytes, bool littleEndian);
    static double bytesToDouble(const unsigned char* bytes, bool littleEndian);
    static std::vector<unsigned char> getRange(const unsigned char* origBytes, int64_t index, int64_t size);
    static std::vector<unsigned char> getRange(const std::vector<unsigned char>& origBytes, int64_t index, int64_t size);
    static std::vector<char> getRange(const char* origBytes, int64_t index, int64_t size);

    static PlistEntry parseBinary(const PlistHelperData& d, int objRef);
    static PlistEntry::dictionary parseBinaryDictionary(const PlistHelperData& d, int objRef);
    static std::vector<PlistEntry> parseBinaryArray(const PlistHelperData& d, int objRef);
    static std::vector<int32_t> getRefsForContainers(const PlistHelperData& d, int objRef);
    static int64_t parseBinaryInt(const PlistHelperData& d, int headerPosition, int& intByteCount);
    static double parseBinaryReal(const PlistHelperData& d, int headerPosition);
    static Date parseBinaryDate(const PlistHelperData& d, int headerPosition);
    static bool parseBinaryBool(const PlistHelperData& d, int headerPosition);
    static std::string parseBinaryString(const PlistHelperData& d, int objRef);
    static std::string parseBinaryUnicode(const PlistHelperData& d, int headerPosition);
    static PlistEntry::data parseBinaryByteArray(const PlistHelperData& d, int headerPosition);
    static std::vector<unsigned char> regulateNullBytes(const std::vector<unsigned char>& origBytes, unsigned int minBytes);
    static void parseTrailer(PlistHelperData& d, const std::vector<unsigned char>& trailer);
    static void parseOffsetTable(PlistHelperData& d, const std::vector<unsigned char>& offsetTableBytes);
    static int32_t getCount(const PlistHelperData& d, int bytePosition, unsigned char headerByte, int& startOffset);

public:
    static void readBinaryPlistFromArray(const char* byteArrayTemp, int64_t size, std::vector<PlistEntry> & message);
    BinaryPlistParser() = default;
};


#endif //PARSER_BINARYPLISTPARSER_H
