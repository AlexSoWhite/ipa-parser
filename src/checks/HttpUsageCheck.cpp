//
// Created by nafanya on 4/21/23.
//

#include "../../include/checks/HttpUsageCheck.h"
#include "../../include/Executable.h"
#include "../../include/utils.h"

bool HttpUsageCheck::isApplicableTo(File &file) {
    return file.getType() == File::MACHO;
}

CheckResult HttpUsageCheck::check(File & file) {
    auto * executable = dynamic_cast<Executable *>(&file);
    std::vector<std::string> result = std::vector<std::string>();
    bool is_first = true;
    for (std::string& s : executable->get_strings("http://")) {
        bool should_analyze = true;
        for (auto& ign : ignored) {
            if (s == ign) {
                should_analyze = false;
            }
        }
        if (should_analyze && std::regex_search(s, http_regex)) {
            if (is_first) {
                is_first = false;
                result.push_back("In file: " + std::string(file.getPath()));
            }
            result.push_back(s);
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
