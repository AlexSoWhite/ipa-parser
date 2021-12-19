//
// Created by alex on 25.11.2021.
//

#ifndef PARSER_FILETYPERESOLVER_H
#define PARSER_FILETYPERESOLVER_H


#include "File.h"
#include <set>
#include <fstream>

class FileTypeResolver {
    static std::map<std::string, File::Types> typeMap;
public:
    FileTypeResolver() = default;
    static File::Types resolve(const std::string&);
    static std::string extractExtension(const std::string & fullPath);
    ~FileTypeResolver() = default;
};


#endif //PARSER_FILETYPERESOLVER_H
