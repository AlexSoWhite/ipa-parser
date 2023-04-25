//
// Created by alex on 10.11.2021.
//

#include "../include/Dispatcher.h"
#include "../include/PlistParser.h"
#include "../include/BinaryPlistParser.h"
#include "../include/JsonFile.h"
#include "../include/Executable.h"
#include "../pugixml-1.11/src/pugixml.hpp"
#include "../include/FileTypeResolver.h"
#include <iostream>
#include <memory>

Dispatcher& Dispatcher::extractIpaName(const std::string & ipaName){
    this->ipaName = ipaName.substr(0, ipaName.size() - 4);
    return *this;
}

const std::string &Dispatcher::getTemporaryDirName() const {
    return temporaryDirName;
}

std::unique_ptr<File> Dispatcher::parsePlist(const std::string& path) {
    std::unique_ptr<PlistFile> plistFile = std::make_unique<PlistFile>();
    plistFile->setPath(path);
    plistFile->setType(File::PLIST);
    plistFile->parse();
    return plistFile;
}

std::unique_ptr<File> Dispatcher::parseJson(const std::string& path) {
    std::unique_ptr<JsonFile> jsonFile = std::make_unique<JsonFile>();
    jsonFile->setPath(path);
    jsonFile->setType(File::PLIST);
    int res = jsonFile->parse();
    if (res == -1) {
        logger.log("error while opening json file");
    } else if (res == 1) {
        logger.log("error while parsing json file");
    }
    return jsonFile;
}

std::unique_ptr<File> Dispatcher::parseBinary(const std::string& path) {
    std::unique_ptr<Executable> e = std::make_unique<Executable>();
    e->setPath(path);
    e->setType(File::MACHO);
    e->read(b_meta_directory, b_disassembles_directory);
    return e;
}

//void Dispatcher::parseNib(const std::string& path) {
//    pugi::xml_document doc;
//    doc.load_file(path);
//}

std::string create_log_string(const std::string& type, const std::string& path) {
    std::stringstream sstream;
    sstream << "[parsing] " << type << " : " << path << "...";
    return sstream.str();
}

void Dispatcher::work(const std::string& path) {
    File::Types type = FileTypeResolver::resolve(path);
    std::unique_ptr<File> file;
    switch (type) {
        case File::PLIST:
            logger.log(create_log_string("plist", path));
            file = parsePlist(path);
            break;
        case File::DATA:
            logger.log(create_log_string("data", path));
            break;
        case File::DIRECTORY:
            logger.log(create_log_string("directory", path));
            break;
        case File::HTML:
            logger.log(create_log_string("html", path));
            break;
        case File::JPG:
            logger.log(create_log_string("jpg", path));
            break;
        case File::JS:
            logger.log(create_log_string("js", path));
            break;
        case File::JSON:
            logger.log(create_log_string("json", path));
            file = parseJson(path);
            break;
        case File::MACHO:
            logger.log(create_log_string("binary", path));
            file = parseBinary(path);
            break;
        case File::M4A:
            logger.log(create_log_string("m4a", path));
            break;
        case File::M4V:
            logger.log(create_log_string("m4v", path));
            break;
        case File::MP3:
            logger.log(create_log_string("mp3", path));
            break;
        case File::MP4:
            logger.log(create_log_string("mp4", path));
            break;
        case File::PNG:
            logger.log(create_log_string("png", path));
            break;
        case File::RES:
            logger.log(create_log_string("res", path));
            break;
        case File::YAML:
            logger.log(create_log_string("yaml", path));
            break;
    }
    if (file != nullptr) {
        for (std::unique_ptr<OwaspCheck>& check : checks) {
            if (check->isApplicableTo(*file)) {
                logger.log("performing " + check->get_name());
                auto result = check->check(*file);
                std::ofstream fout(result.get_name(), std::ios_base::app);
                result.print(fout);
                fout.close();
            }
        }
    }
}
