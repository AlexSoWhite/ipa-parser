//
// Created by nafanya on 4/20/23.
//

#include "../../include/checks/MD5UsageCheck.h"
#include "../../include/Executable.h"

bool MD5UsageCheck::isApplicableTo(File &file) {
    return file.getType() == File::MACHO;
}

CheckResult MD5UsageCheck::check(File & file) {
    auto * executable = dynamic_cast<Executable *>(&file);
    std::vector<std::string> result = std::vector<std::string>();
    bool is_first = true;
    for (std::string& string : executable->get_sym_strings()) {
        if (string.find("MD5") != std::string::npos) {
            if (is_first) {
                is_first = false;
                result.push_back("In file: " + std::string(file.getPath()));
            }
            result.push_back(string);
        }
    }
    ViolationPointVector v = ViolationPointVector();
    v.points = result;
    return {
            !result.empty(),
            this->name,
            this->description,
            v,
            this->how_to_fix,
            this->warning_message
    };
}
