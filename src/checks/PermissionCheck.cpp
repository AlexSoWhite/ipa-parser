//
// Created by nafanya on 4/20/23.
//

#include "../../include/checks/PermissionCheck.h"
#include "../../include/utils.h"

bool PermissionCheck::isApplicableTo(File& file) {
    return file.getType() == File::PLIST &&
        count_segments(file.getPath()) == 3 &&
        last_path_segment(file.getPath()) == "Info.plist";
}

CheckResult PermissionCheck::check(File& file) {
    auto* plistFile = dynamic_cast<PlistFile*>(&file);
    std::vector<std::string> result = std::vector<std::string>();
    auto dict = std::get<PlistEntry::dictionary>(plistFile->getData()[0].entry);
    bool is_first = true;
    for (const auto &entry: dict) {
        for (const auto &sus_perm: sus_permissions) {
            if (entry.second.type == PlistEntry::STRING) {
                if (strcmp(entry.first.c_str(), sus_perm.c_str()) == 0) {
                    if (is_first) {
                        is_first = false;
                        result.push_back("In file: " + std::string(file.getPath()));
                    }
                    result.push_back(entry.first + ": " + std::get<PlistEntry::string>(entry.second.entry));
                }
            }
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
