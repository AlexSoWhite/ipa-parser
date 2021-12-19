//
// Created by alex on 14.11.2021.
//

#include <iostream>
#include "../include/PlistEntry.h"

static int depth = 0;

const std::map<std::string, PlistEntry::PlistEntryTypes> PlistEntry::typeMap =  {
        {"dict", PlistEntryTypes::DICT},
        {"array", PlistEntryTypes::ARRAY},
        {"string", PlistEntryTypes::STRING},
        {"integer", PlistEntryTypes::INTEGER},
        {"real", PlistEntryTypes::REAL},
        {"false", PlistEntryTypes::FALSE},
        {"true", PlistEntryTypes::TRUE},
        {"data", PlistEntryTypes::DATA},
        {"date", PlistEntryTypes::DATE}
};

std::ostream & operator << (std::ostream & os, const PlistEntry & plistEntry) {

    switch (plistEntry.type) {

        case PlistEntry::DICT:
            depth++;
            os << "(dict)";
            os << std::endl;
            for (const auto & e : std::get<PlistEntry::dictionary>(plistEntry.entry)) {
                for (int i = 0; i < depth; i ++) {
                    os<<"\t";
                }
                os << e.first << ": " << e.second;
            }
            depth--;
            break;
        case PlistEntry::ARRAY:
            depth++;
            os << "(array)";
            os<<std::endl;
            for (const auto & e : std::get<PlistEntry::array>(plistEntry.entry)) {
                for (int i = 0; i < depth; i ++) {
                    os<<"\t";
                }
                os << e;
            }
            depth--;
            break;
        case PlistEntry::STRING:
            os << std::get<PlistEntry::string>(plistEntry.entry) << std::endl;
            break;
        case PlistEntry::INTEGER:
            os << std::get<PlistEntry::integer>(plistEntry.entry) << std::endl;
            break;
        case PlistEntry::REAL:
            os << std::get<PlistEntry::real>(plistEntry.entry) << std::endl;
            break;
        case PlistEntry::FALSE:
        case PlistEntry::TRUE:
            os << std::get<PlistEntry::boolean>(plistEntry.entry) << std::endl;
            break;
        case PlistEntry::DATA:
            for (const char c : std::get<PlistEntry::data>(plistEntry.entry)) {
                os << c;
            }
            os << std::endl;
            break;
        case PlistEntry::DATE:
            os << "date" << std::endl;
            break;
    }
    return os;
}