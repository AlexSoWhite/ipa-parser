//
// Created by alex on 10.11.2021.
//

#ifndef PARSER_FILE_H
#define PARSER_FILE_H


#include <string>
#include <map>

class File {
public:
    enum Types{
        DATA,
        DIRECTORY,
        HTML,
        JPG,
        JS,
        JSON,
        MACHO,
        M4A,
        M4V,
        MP3,
        MP4,
        PLIST,
        PNG,
        RES,
        YAML
    };
    File() = default;
    virtual ~File() = default;
    File& setSize(const unsigned int size) { this->size = size; return *this; }
    File& setType(Types type) { this->type = type; return *this; }
    File& setPath(const std::string& path) { this->path = path.c_str(); return *this; };
    [[nodiscard]] const char * getPath() const { return path; }
    [[nodiscard]] unsigned int getSize() const { return size; }
    [[nodiscard]] Types getType() const { return type; }
protected:
    const char * path;
    unsigned int size;
    Types type;
};


#endif //PARSER_FILE_H
