//
// Created by alex on 25.11.2021.
//

#include "../include/FileTypeResolver.h"
#include <magic.h>
#include <iostream>

std::map<std::string, File::Types> FileTypeResolver::typeMap = {
        {".data", File::Types::DATA},
        {".html", File::Types::HTML},
        {".jpg", File::Types::JPG},
        {".js", File::Types::JS},
        {".json", File::Types::JSON},
        {".m4a", File::Types::M4A},
        {".m4v", File::Types::M4V},
        {".mp3", File::Types::MP3},
        {".mp4", File::Types::MP4},
        {".plist", File::Types::PLIST},
        {".png", File::Types::PNG},
        {".yaml", File::Types::YAML}
};

File::Types FileTypeResolver::resolve(const std::string& path) {
    std::string extension = extractExtension(path);
    if (!extension.empty()) {
        if (typeMap.contains(extension)) {
            return typeMap.at(extension);
        } else {
            return File::RES;
        }
    }
    else {
        magic_t myt = magic_open(MAGIC_CONTINUE | MAGIC_ERROR);
        magic_load(myt, nullptr);
        std::string a = std::string(magic_file(myt, path.c_str()));
        if (a == "directory") {
            magic_close(myt);
            return File::DIRECTORY;
        }
        int commaPos = a.find(',');
        int newlinePos = a.find("\\012");
        if (commaPos < 0) {
            // substr[\012]
            a = a.substr(0, newlinePos);
        } else if (newlinePos < 0) {
            // substr[',']
            a = a.substr(0, commaPos);
        } else {
            // substr[min(,\012)]
            a = a.substr(0, std::min(commaPos, newlinePos));
        }
        a = a.substr(0, a.find(' '));
        if(a == "Mach-O") {
            magic_close(myt);
            return File::MACHO;
        }
        magic_close(myt);
    }
    return File::RES;
}

std::string FileTypeResolver::extractExtension(const std::string &fullPath) {
    std::string tempFname = fullPath.substr(fullPath.find_last_of('/'));
    if (tempFname.find_last_of('.') != -1) {
        return tempFname.substr(tempFname.find_last_of('.'));
    }
    return "";
}
