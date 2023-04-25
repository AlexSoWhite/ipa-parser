//
// Created by nafanya on 4/21/23.
//

#ifndef PARSER_HTTPUSAGECHECK_H
#define PARSER_HTTPUSAGECHECK_H


#include "OwaspCheck.h"
#include <regex>

class HttpUsageCheck : public OwaspCheck {
    std::vector<std::string> ignored = {
            "$http://ocsp.apple.com/ocsp03-aipca040",
            "(http://ocsp.apple.com/ocsp03-applerootca0.",
            "http://crl.apple.com/root.crl0",
            "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">"
    };
    std::regex http_regex = std::regex("http:\\/\\/([\\w_-]+(?:(?:\\.[\\w_-]+)+))([\\w.,@?^=%&:\\/~+#-]*[\\w@?^=%&\\/~+#-])");
public:
    HttpUsageCheck() {
        this->name = "http usage check";
        this->description = "insecure network protocol";
        this->how_to_fix = "";
        this->warning_message = "http connection is not secure, make sure that nothing sensitive is transferred via this protocol";
    }
    bool isApplicableTo(File&) override;
    [[nodiscard]] CheckResult check(File&) override;
};


#endif //PARSER_HTTPUSAGECHECK_H
