//
// Created by alex on 10.11.2021.
//

#include "../include/Dispatcher.h"
#include "../include/PlistFile.h"
#include "../include/PlistParser.h"
#include "../include/BinaryPlistParser.h"
#include "../include/JsonFile.h"
#include "../include/Executable.h"
#include "../pugixml-1.11/src/pugixml.hpp"
#include "../include/FileTypeResolver.h"
#include <memory>

Dispatcher& Dispatcher::setTemporaryDirName(const std::string &dirName) {
    this->temporaryDirName = dirName;
    return *this;
}

Dispatcher& Dispatcher::extractIpaName(const std::string & ipaName){
    this->ipaName = ipaName.substr(0, ipaName.size() - 4);
    return *this;
}

const std::string &Dispatcher::getTemporaryDirName() const {
    return temporaryDirName;
}

void Dispatcher::parsePlist(const std::string& path) {
    std::unique_ptr<PlistFile> plistFile = std::make_unique<PlistFile>();
    plistFile->setPath(path);
    plistFile->setType(File::PLIST);
    plistFile->parse();
}

void Dispatcher::parseJson(const std::string& path) {
    std::unique_ptr<JsonFile> jsonFile = std::make_unique<JsonFile>();
    jsonFile->setPath(path);
    jsonFile->setType(File::PLIST);
    jsonFile->parse();
}

void Dispatcher::parseBinary(const std::string& path) {
    std::unique_ptr<Executable> e = std::make_unique<Executable>();
    e->setPath(path);
    e->setType(File::PLIST);
    e->read();
}

//void Dispatcher::parseNib(const std::string& path) {
//    pugi::xml_document doc;
//    doc.load_file(path);
//}

void log(std::string type, std::string path) {
    std::cout << "[parsing] " << type << " : " << path << "..." << std::endl;
}

void Dispatcher::work(const std::string& path) {
    File::Types type = FileTypeResolver::resolve(path);
    switch (type) {
        case File::PLIST:
            log("plist", path);
            parsePlist(path);
            break;
        case File::DATA:
            log("data", path);
            break;
        case File::DIRECTORY:
            log("directory", path);
            break;
        case File::HTML:
            log("html", path);
            break;
        case File::JPG:
            log("jpg", path);
            break;
        case File::JS:
            log("js", path);
            break;
        case File::JSON:
            log("json", path);
            parseJson(path);
            break;
        case File::MACHO:
            log("binary", path);
            parseBinary(path);
            break;
        case File::M4A:
            log("m4a", path);
            break;
        case File::M4V:
            log("m4v", path);
            break;
        case File::MP3:
            log("mp3", path);
            break;
        case File::MP4:
            log("mp4", path);
            break;
        case File::PNG:
            log("png", path);
            break;
        case File::RES:
            log("res", path);
            break;
        case File::YAML:
            log("yaml", path);
            break;
    }
}
