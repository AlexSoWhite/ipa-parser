//
// Created by alex on 14.11.2021.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/locale/encoding_utf.hpp>
#include "../include/BinaryPlistParser.h"

template<typename T>
const T* vecData(const std::vector<T>& vec)
{
    return (!vec.empty()) ? &vec[0] : nullptr;
}

inline bool hostLittleEndian()
{
    union { uint32_t x; uint8_t c[4]; } u{};
    u.x = 0xab0000cd;
    return u.c[0] == 0xcd;
}

void BinaryPlistParser::readBinaryPlistFromArray(
    const char *byteArrayTemp,
    int64_t size,
    std::vector<PlistEntry> &content
) {
    const auto* byteArray = (const unsigned char*) byteArrayTemp;
    if (!byteArray || (size == 0))
        return;

    PlistHelperData d;
    parseTrailer(d, getRange(byteArray, size - 32, 32));

    d._objectTable = getRange(byteArray, 0, d._offsetTableOffset);
    std::vector<unsigned char> offsetTableBytes = getRange(byteArray, d._offsetTableOffset, size - d._offsetTableOffset - 32);

    parseOffsetTable(d, offsetTableBytes);

    content.emplace_back(parseBinary(d, 0));

}

PlistEntry BinaryPlistParser::parseBinary(const PlistHelperData &d, int objRef) {
    unsigned char header = d._objectTable[d._offsetTable[objRef]];

    PlistEntry result;

    bool r;

    switch (header & 0xF0)
    {
        case 0x00:
            r = parseBinaryBool(d, d._offsetTable[objRef]);
            if (r) {
                result = PlistEntry(
                        PlistEntry::PlistEntryTypes::TRUE,
                        r
                );
            } else {
                result = PlistEntry(
                        PlistEntry::PlistEntryTypes::FALSE,
                        r
                );
            }
            break;
        case 0x10:
            int intByteCount;
            result = PlistEntry(
                    PlistEntry::INTEGER,
                    parseBinaryInt(d, d._offsetTable[objRef], intByteCount)
            );
            break;
        case 0x20:
            result = PlistEntry(
                    PlistEntry::REAL,
                    parseBinaryReal(d, d._offsetTable[objRef])
            );
            break;
        case 0x30:
            result = PlistEntry(
                    PlistEntry::DATE,
                    parseBinaryDate(d, d._offsetTable[objRef])
            );
            break;
        case 0x40:
            result = PlistEntry(
                    PlistEntry::DATA,
                    parseBinaryByteArray(d, d._offsetTable[objRef])
            );
            break;
        case 0x50:
            result = PlistEntry(
                    PlistEntry::STRING,
                    parseBinaryString(d, d._offsetTable[objRef])
            );
            break;
        case 0x60:
            result = PlistEntry(
                    PlistEntry::STRING,
                    parseBinaryUnicode(d, d._offsetTable[objRef])
            );
            break;
        case 0xD0:
            result = PlistEntry(
                    PlistEntry::DICT,
                    parseBinaryDictionary(d, objRef)
            );
            break;
        case 0xA0:
            result = PlistEntry(
                    PlistEntry::ARRAY,
                    parseBinaryArray(d, objRef)
            );
            break;
    }
    return result;
}

bool BinaryPlistParser::parseBinaryBool(const PlistHelperData& d, int headerPosition)
{
    unsigned char header = d._objectTable[headerPosition];
    bool value;
    if(header == 0x09)
        value = true;
    else if (header == 0x08)
        value = false;
    else if (header == 0x00)
    {
        // null byte, not sure yet what to do with this.  It's in the spec but we
        // have never encountered it.

        // error
    }
    else if (header == 0x0F)
    {
        // fill byte, not sure yet what to do with this.  It's in the spec but we
        // have never encountered it.

        // error
    }
    else
    {
        std::stringstream ss;
        ss<<"Plist: unknown header "<<header;
        // error
    }

    return value;
}

int64_t BinaryPlistParser::parseBinaryInt(const PlistHelperData& d, int headerPosition, int& intByteCount)
{
    unsigned char header = d._objectTable[headerPosition];
    intByteCount = 1 << (header & 0xf);
    std::vector<unsigned char> buffer = getRange(d._objectTable, headerPosition + 1, intByteCount);
    reverse(buffer.begin(), buffer.end());

    return bytesToInt<int64_t>(vecData(regulateNullBytes(buffer, 8)), hostLittleEndian());
}

double BinaryPlistParser::parseBinaryReal(const PlistHelperData& d, int headerPosition)
{
    unsigned char header = d._objectTable[headerPosition];
    int byteCount = 1 << (header & 0xf);
    std::vector<unsigned char> buffer = getRange(d._objectTable, headerPosition + 1, byteCount);
    reverse(buffer.begin(), buffer.end());

    return bytesToDouble(vecData(regulateNullBytes(buffer, 8)), hostLittleEndian());
}

Date BinaryPlistParser::parseBinaryDate(const PlistHelperData& d, int headerPosition)
{
    // date always an 8 byte float starting after full byte header
    std::vector<unsigned char> buffer = getRange(d._objectTable, headerPosition + 1, 8);

    Date date;

    // Date is stored as Apple Epoch and big endian.
    date.setTimeFromAppleEpoch(bytesToDouble(vecData(buffer), false));

    return date;
}

std::vector<char> BinaryPlistParser::parseBinaryByteArray(const PlistHelperData& d, int headerPosition)
{
    unsigned char headerByte = d._objectTable[headerPosition];
    int byteStartPosition;
    int32_t byteCount = getCount(d, headerPosition, headerByte, byteStartPosition);
    byteStartPosition += headerPosition;

    return getRange((const char*) vecData(d._objectTable), byteStartPosition, byteCount);
}

std::string BinaryPlistParser::parseBinaryString(const PlistHelperData& d, int headerPosition)
{
    unsigned char headerByte = d._objectTable[headerPosition];
    int charStartPosition;
    int32_t charCount = getCount(d, headerPosition, headerByte, charStartPosition);
    charStartPosition += headerPosition;

    std::vector<unsigned char> characterBytes = getRange(d._objectTable, charStartPosition, charCount);
    std::string buffer = std::string((char*) vecData(characterBytes), characterBytes.size());
    return buffer;
}

std::string BinaryPlistParser::parseBinaryUnicode(const PlistHelperData& d, int headerPosition)
{
    unsigned char headerByte = d._objectTable[headerPosition];
    int charStartPosition;
    int32_t charCount = getCount(d, headerPosition, headerByte, charStartPosition);
    charStartPosition += headerPosition;

    std::vector<unsigned char> characterBytes = getRange(d._objectTable, charStartPosition, charCount * 2);
    if (hostLittleEndian()) {
        if (! characterBytes.empty()) {
            for (std::size_t i = 0, n = characterBytes.size(); i < n - 1; i += 2)
                std::swap(characterBytes[i], characterBytes[i + 1]);
        }
    }

    auto *u16chars = (int16_t*) vecData(characterBytes);
    std::size_t u16len = characterBytes.size() / 2;
    std::string result = boost::locale::conv::utf_to_utf<char, int16_t>(u16chars, u16chars + u16len, boost::locale::conv::stop);
    return result;
}

std::map<std::string, PlistEntry> BinaryPlistParser::parseBinaryDictionary(const PlistHelperData& d, int objRef)
{
    using namespace std;
    vector<int32_t> refs = getRefsForContainers(d, objRef);
    int32_t refCount = refs.size() / 2;

    std::map<std::string, PlistEntry> dict;
    for (int i = 0; i < refCount; i++)
    {
        PlistEntry keyAny = parseBinary(d, refs[i]);

        try
        {
            std::string key = std::get<std::string>(keyAny.entry);
            dict[key] =  parseBinary(d, refs[i + refCount]);
        }
        catch(boost::bad_any_cast& )
        {
            // error
        }
    }

    return dict;
}

std::vector<PlistEntry> BinaryPlistParser::parseBinaryArray(const PlistHelperData& d, int objRef)
{
    using namespace std;
    vector<int32_t> refs = getRefsForContainers(d, objRef);
    int32_t refCount = refs.size();

    std::vector<PlistEntry> array;
    array.reserve(refCount);
    for(int i = 0; i < refCount; ++i)
        array.push_back(parseBinary(d, refs[i]));

    return array;
}

void BinaryPlistParser::parseTrailer(PlistHelperData& d, const std::vector<unsigned char>& trailer)
{
    d._offsetByteSize = bytesToInt<int32_t>(vecData(regulateNullBytes(getRange(trailer, 6, 1), 4)), hostLittleEndian());
    d._objRefSize = bytesToInt<int32_t>(vecData(regulateNullBytes(getRange(trailer, 7, 1), 4)), hostLittleEndian());

    std::vector<unsigned char> refCountBytes = getRange(trailer, 12, 4);
//	std::reverse(refCountBytes.begin(), refCountBytes.end());
    d._refCount = bytesToInt<int32_t>(vecData(refCountBytes), false);

    std::vector<unsigned char> offsetTableOffsetBytes = getRange(trailer, 24, 8);
//	std::reverse(offsetTableOffsetBytes.begin(), offsetTableOffsetBytes.end());
    d._offsetTableOffset = bytesToInt<int64_t>(vecData(offsetTableOffsetBytes), false);
}

void BinaryPlistParser::parseOffsetTable(PlistHelperData& d, const std::vector<unsigned char>& offsetTableBytes)
{
    for (unsigned int i = 0; i < offsetTableBytes.size(); i += d._offsetByteSize)
    {
        std::vector<unsigned char> temp = getRange(offsetTableBytes, i, d._offsetByteSize);
        std::reverse(temp.begin(), temp.end());
        d._offsetTable.push_back(
                bytesToInt<int32_t>(
                        vecData(regulateNullBytes(temp, 4)), hostLittleEndian()));
    }
}

template <typename IntegerType>
IntegerType BinaryPlistParser::bytesToInt(const unsigned char* bytes, bool littleEndian)
{
    IntegerType result = 0;
    if (littleEndian)
        for (int n = sizeof( result ) - 1; n >= 0; n--)
            result = (result << 8) + bytes[n];
    else
        for (unsigned n = 0; n < sizeof( result ); n++)
            result = (result << 8) + bytes[n];
    return result;
}

double BinaryPlistParser::bytesToDouble(const unsigned char* bytes, bool littleEndian)
{
    double result;
    int numBytes = sizeof(double);
    if(littleEndian)
        memcpy( &result, bytes, numBytes);
    else
    {
        std::vector<unsigned char> bytesReverse(numBytes);
        std::reverse_copy(bytes, bytes + numBytes, bytesReverse.begin());
        memcpy( &result, vecData(bytesReverse), numBytes);
    }
    return result;
}

std::vector<unsigned char> BinaryPlistParser::regulateNullBytes(const std::vector<unsigned char>& origBytes, unsigned int minBytes)
{

    std::vector<unsigned char> bytes(origBytes);
    while((bytes.back() == 0) && (bytes.size() > minBytes))
        bytes.pop_back();

    while(bytes.size() < minBytes)
        bytes.push_back(0);

    return bytes;
}

std::vector<unsigned char> BinaryPlistParser::getRange(const unsigned char* origBytes, int64_t index, int64_t size)
{
    std::vector<unsigned char> result((std::vector<unsigned char>::size_type)size);
    std::copy(origBytes + index, origBytes + index + size, result.begin());
    return result;
}

std::vector<char> BinaryPlistParser::getRange(const char* origBytes, int64_t index, int64_t size)
{
    std::vector<char> result((std::vector<char>::size_type)size);
    std::copy(origBytes + index, origBytes + index + size, result.begin());
    return result;
}

std::vector<unsigned char> BinaryPlistParser::getRange(const std::vector<unsigned char>& origBytes, int64_t index, int64_t size)
{
    if((index + size) > (int64_t) origBytes.size()){
        // out of bounds
    }
    return getRange(vecData(origBytes), index, size);
}

int32_t BinaryPlistParser::getCount(const PlistHelperData& d, int bytePosition, unsigned char headerByte, int& startOffset)
{
    unsigned char headerByteTrail = headerByte & 0xf;
    if (headerByteTrail < 15)
    {
        startOffset = 1;
        return headerByteTrail;
    }
    else
    {
        auto count = (int32_t)parseBinaryInt(d, bytePosition + 1, startOffset);
        startOffset += 2;
        return count;
    }
}

std::vector<int32_t> BinaryPlistParser::getRefsForContainers(const PlistHelperData& d, int objRef)
{
    using namespace std;
    int32_t refCount;
    int refStartPosition;
    refCount = getCount(d, d._offsetTable[objRef], d._objectTable[d._offsetTable[objRef]], refStartPosition);
    refStartPosition += d._offsetTable[objRef];

    vector<int32_t> refs;
    int mult = 1;
    if((((unsigned char) d._objectTable[d._offsetTable[objRef]]) & 0xF0) == 0xD0)
        mult = 2;
    for (int i = refStartPosition; i < refStartPosition + refCount * mult * d._objRefSize; i += d._objRefSize)
    {
        std::vector<unsigned char> refBuffer = getRange(d._objectTable, i, d._objRefSize);
        reverse(refBuffer.begin(), refBuffer.end());
        refs.push_back(bytesToInt<int32_t>(vecData(regulateNullBytes(refBuffer, 4)), hostLittleEndian()));
    }

    return refs;
}