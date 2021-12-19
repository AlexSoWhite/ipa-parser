//
// Created by alex on 20.11.2021.
//

#include <fstream>
#include "../include/JsonFile.h"

//TODO error catching
void JsonFile::parse() {
    std::ifstream fin(this->path);
    if (!fin.is_open()) {
        return;
    }
    nlohmann::json j;
    fin >> j;
    fin.close();
    this->data = j;
    std::ofstream fout;
    fout.open(this->path);
    fout << std::setw(4) << j;
    fout.close();
}
