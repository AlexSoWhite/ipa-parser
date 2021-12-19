//
// Created by alex on 10.11.2021.
//

#include "../include/FileBundleParser.h"
#include <filesystem>

void FileBundleParser::parse() {
    for (const auto &entry : std::filesystem::recursive_directory_iterator(this->temporaryDirName)) {
        if(entry.is_directory()){
            this->fileNames.emplace_back(entry.path().string()+"/");
        } else {
            this->fileNames.emplace_back(entry.path().string());
        }
    }
}
