//
// Created by alex on 20.11.2021.
//

#include <fstream>
#include <iostream>
#include "../include/JsonFile.h"

int JsonFile::parse() {
    std::ifstream fin(this->path);
    if (!fin.is_open()) {
        return -1;
    }
    nlohmann::json j;
    int status = 1;
    try {
        fin >> j;
        this->data = j;
        std::ofstream fout;
        fout.open(this->path);
        fout << std::setw(4) << j;
        fout.close();
    } catch (const nlohmann::json::parse_error& e) {
        std::cout << "error while parsing json" << std::endl;
        status = 0;
    }
    fin.close();
    return status;
}
