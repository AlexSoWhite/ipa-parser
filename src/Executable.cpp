//
// Created by alex on 21.11.2021.
//

#include <iostream>
#include "../include/Executable.h"
#include "../include/utils.h"
#include "SwiftDecompiler.h"
#include <LIEF/LIEF.hpp>

int Executable::read(const std::string& b_meta_directory, const std::string& b_decompiled_directory) {
    int status = 0;
    auto binaryReader = BinaryReader(path);
    std::string name = last_path_segment(path);
    normalize_spaces(name);
    this->binary = std::make_unique<Binary>(
                Binary(
                        binaryReader.getCodes(),
                        binaryReader.getSymtab(),
                        binaryReader.getStringTab()
                )
        );
    std::ofstream fout(b_decompiled_directory + "/" + name);
    this->binary->printBinary(fout);
    std::ofstream fout_s("tmp/sym_strings/" + name);
    sym_strings = this->binary->get_sym_strings();
    fout.close();
    fout_s.close();
    try {
        this->macho_lief = LIEF::MachO::Parser::parse(this->path);
        std::cout << "writing metadata to file..." <<std::endl;
        fout.open(b_meta_directory + "/" + last_path_segment(this->path));
        fout << *this->macho_lief << std::endl;
        fout.close();
    } catch (const LIEF::exception& err) {
        std::cerr << err.what() << std::endl;
        status = 1;
    }

    return status;
}
