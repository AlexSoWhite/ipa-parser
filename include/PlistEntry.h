//
// Created by alex on 14.11.2021.
//

#ifndef PARSER_PLISTENTRY_H
#define PARSER_PLISTENTRY_H


#include <map>
#include <variant>
#include <vector>
#include "PlistDate.h"

// class that grants access particular single plist entry and defines helpful types for parsing
class PlistEntry{
public:

    enum PlistEntryTypes {
        DICT,
        ARRAY,
        STRING,
        INTEGER,
        REAL,
        FALSE,
        TRUE,
        DATA,
        DATE
    };

    static const std::map<std::string, PlistEntryTypes> typeMap;

    typedef std::map<std::string, PlistEntry> dictionary;
    typedef std::vector<PlistEntry> array;
    typedef std::string string;
    typedef int64_t integer;
    typedef double real;
    typedef bool boolean;
    typedef std::vector<char> data;
    typedef Date date;

    typedef std::variant<
            dictionary,
            array,
            string,
            integer,
            real,
            boolean,
            data,
            date
            > anyEntry;

    PlistEntry() = default;

    PlistEntry(PlistEntryTypes type, const anyEntry & entry) {
        this->type = type;
        this->entry = entry;
    }

    friend class PlistParser;
    friend class BinaryPlistParser;
    friend class PlistFile;

    friend std::ostream & operator << (std::ostream & os, const PlistEntry & plistEntry);

private:
    // actual data
    PlistEntryTypes type;
    anyEntry entry;
};

#endif //PARSER_PLISTENTRY_H
