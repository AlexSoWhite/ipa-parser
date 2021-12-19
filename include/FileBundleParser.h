//
// Created by alex on 10.11.2021.
//

#ifndef PARSER_FILEBUNDLEPARSER_H
#define PARSER_FILEBUNDLEPARSER_H

#include <string>
#include <vector>

class FileBundleParser {
    std::string temporaryDirName;
    std::vector<std::string> fileNames;
public:
    FileBundleParser() = delete;
    explicit FileBundleParser(const std::string & name) { this->temporaryDirName = name; this->fileNames = {}; }
    void parse();
    [[nodiscard]] const std::vector<std::string> & getFileNames() const { return fileNames; }
};


#endif //PARSER_FILEBUNDLEPARSER_H
