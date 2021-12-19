//
// Created by alex on 10.11.2021.
//

#include "../include/Unzipper.h"
#include <sstream>
#include <cstdlib>

static std::string build_command(const std::string & directoryName, const std::string & tempDirectoryName) {
    std::stringstream unzip_command;
    unzip_command << "unzip -d " << tempDirectoryName << " " << directoryName << " > unzip_log";
    return unzip_command.str();
}

static char * get_command(const std::string & rawCommand) {
    char * command = (char *) calloc(rawCommand.size(), sizeof(char));
    for (int i = 0; i <rawCommand.size(); i++) {
        command[i] = rawCommand[i];
    }
    return command;
}

void Unzipper::unzip(const std::string & directoryName) {
    char * command = get_command(build_command(directoryName, this->temporaryDirName));
    system(command);
    free(command);
}

