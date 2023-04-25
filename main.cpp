//
// Created by alex on 09.11.2021.
//

#include <iostream>
#include <regex>
#include <filesystem>
#include "include/Unzipper.h"
#include "include/Dispatcher.h"
#include "include/FileBundleParser.h"

int main(int argc, const char** argv) {

    if (argc != 2) {
        std::cout<<"usage: ./parser [path-to-ipa]"<<std::endl;
        return 1;
    }

    std::regex r = std::regex(".ipa");
    if (!std::regex_search(argv[1], r)) {
        std::cout<<"error: not .ipa file"<<std::endl;
        return 1;
    }

    // controller
    std::filesystem::create_directory("tmp");
    Dispatcher dispatcher = Dispatcher("tmp", "dispatcher_logs");
    dispatcher.extractIpaName(argv[1]);

    // unzip files to temporary directory
    Unzipper unzipper = Unzipper(dispatcher.getTemporaryDirName());
    unzipper.unzip(argv[1]);

    // get filenames
    FileBundleParser fileBundleParser = FileBundleParser(dispatcher.getTemporaryDirName());
    fileBundleParser.parse();

    for (const auto & p: fileBundleParser.getFileNames()) {
        dispatcher.work(p);
    }

    return 0;
}
