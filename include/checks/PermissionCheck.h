//
// Created by nafanya on 4/20/23.
//

#ifndef PARSER_PERMISSIONCHECK_H
#define PARSER_PERMISSIONCHECK_H


#include <string>
#include "OwaspCheck.h"
#include "../PlistFile.h"

class PermissionCheck : public OwaspCheck {
    std::vector<std::string> sus_permissions =
                {
                    "NSCameraUsageDescription",
                    "NSMicrophoneUsageDescription",
                    "NSCalendarsUsageDescription",
                    "NSContactsUsageDescription",
                    "NSFaceIDUsageDescription"
                };
public:
    PermissionCheck() {
        this->name = "permission check";
        this->description = "suspicious permissions";
        this->how_to_fix = "";
        this->warning_message = "make sure permissions requests are described correctly";
    };
    bool isApplicableTo(File&) override;
    [[nodiscard]] CheckResult check(File&) override;
};


#endif //PARSER_PERMISSIONCHECK_H
