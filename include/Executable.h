//
// Created by alex on 21.11.2021.
//

#ifndef PARSER_EXECUTABLE_H
#define PARSER_EXECUTABLE_H


#include "File.h"
#include <list>
#include <utility>
#include <vector>
#include <memory>
#include <fstream>
#include <LIEF/LIEF.hpp>
#include <SwiftDecompiler.h>

class Executable : public File {
    std::unique_ptr<LIEF::MachO::FatBinary> macho_lief;
    std::unique_ptr<Binary> binary;
    std::vector<std::string> sym_strings;
public:
    std::vector<std::string> get_sym_strings() {
        return sym_strings;
    };
    std::vector<std::string> get_strings(const std::string& grep_str) {
        return binary->get_strings(path, grep_str);
    }
    int read(const std::string&, const std::string&);
    ~Executable() override = default;
};


#endif //PARSER_EXECUTABLE_H
