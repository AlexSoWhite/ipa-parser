//
// Created by nafanya on 4/20/23.
//

#ifndef PARSER_MD5USAGECHECK_H
#define PARSER_MD5USAGECHECK_H


#include "OwaspCheck.h"

class MD5UsageCheck : public OwaspCheck {
public:
    MD5UsageCheck() {
        this->name = "md5 usage check";
        this->description = "insecure crypto algorithm";
        this->how_to_fix = "";
        this->warning_message = "md5 should not be used for cryptographic purposes";
    };
    bool isApplicableTo(File&) override;
    [[nodiscard]] CheckResult check(File&) override;
};


#endif //PARSER_MD5USAGECHECK_H
