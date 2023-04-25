//
// Created by alex on 10.11.2021.
//

#ifndef PARSER_DISPATCHER_H
#define PARSER_DISPATCHER_H


#include <string>
#include <utility>
#include "File.h"
#include "Logger.h"
#include "checks/OwaspCheck.h"
#include "checks/PermissionCheck.h"
#include "checks/MD5UsageCheck.h"
#include "checks/HttpUsageCheck.h"
#include <filesystem>

class Dispatcher {
    std::string temporaryDirName;
    std::string ipaName;
    std::string b_meta_directory = "tmp/binaries_meta";
    std::string b_disassembles_directory = "tmp/binaries_disassembled";
    Logger logger;
    std::vector<std::unique_ptr<OwaspCheck>> checks;
public:
    Dispatcher(const std::string& tmpDirName, const std::string& logPathName) : logger(logPathName) {
        this->temporaryDirName = tmpDirName;
        std::filesystem::create_directory(b_meta_directory);
        std::filesystem::create_directory(b_disassembles_directory);
        std::filesystem::create_directory(tmpDirName + "/strings");
        std::filesystem::create_directory(tmpDirName + "/sym_strings");
        checks.push_back(std::make_unique<PermissionCheck>(PermissionCheck()));
        checks.push_back(std::make_unique<MD5UsageCheck>(MD5UsageCheck()));
        checks.push_back(std::make_unique<HttpUsageCheck>(HttpUsageCheck()));
        for (auto& check : checks) {
            std::filesystem::remove(check->get_name());
        }
    }
    Dispatcher& extractIpaName(const std::string & ipaName);
    [[nodiscard]] const std::string &getTemporaryDirName() const;
    [[nodiscard]] const std::string &getIpaName() const { return this->ipaName; }
    void work(const std::string&);
    std::unique_ptr<File> parsePlist(const std::string&);
    std::unique_ptr<File> parseJson(const std::string&);
    std::unique_ptr<File> parseBinary(const std::string&);
};


#endif //PARSER_DISPATCHER_H
