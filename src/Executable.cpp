//
// Created by alex on 21.11.2021.
//

#include <fstream>
#include <iostream>
#include "../include/Executable.h"

Executable::Executable(std::string strPath) {
    this->path = strPath.c_str();
    this->read();
}

std::string lastPathSegm(const char * path) {
    std::string lp(path);
    lp = lp.substr(lp.find_last_of('/')+1);
    return lp;
}

int Executable::read() {
    try {
        this->macho = LIEF::MachO::Parser::parse(this->path);
        std::cout << "writing to file..." <<std::endl;
        std::ofstream fout;
        std::string prefix = "smhBinE/";
        fout.open(prefix + lastPathSegm(this->path));
        fout << *this->macho << std::endl;
        fout.close();
    } catch (const LIEF::exception& err) {
        std::cerr << err.what() << std::endl;
    }

    return 0;
}
