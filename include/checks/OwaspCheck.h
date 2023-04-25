//
// Created by nafanya on 4/20/23.
//

#ifndef PARSER_OWASPCHECK_H
#define PARSER_OWASPCHECK_H

#include <vector>
#include "../File.h"
#include <ostream>

// класс-обертка для точек нарушений правила
class ViolationPointVector {
public:
    std::vector<std::string> points;
};

// класс отчета
class CheckResult {
    bool violated;
    std::string name;
    std::string description;
    ViolationPointVector violation_points;
    std::string how_to_fix;
    std::string warning_message;
public:
    CheckResult(
            bool _violated,
            const std::string& _name,
            const std::string& _description,
            const ViolationPointVector& _violation_points,
            const std::string& _how_to_fix,
            const std::string& _warning_message
    ) : violated(_violated) {
        this->name = _name;
        this->description = _description;
        this->violation_points = _violation_points;
        this->how_to_fix = _how_to_fix;
        this->warning_message = _warning_message;
    }
    std::string get_name() {
        return this->name;
    }
    void print(std::ostream& out) {
        if (this->violated) {
            out << this->name << ", violated " << std::endl;

            out << std::endl << "violation_points:" << std::endl;

            for (const std::string& point : this->violation_points.points) {
                out << point << std::endl;
            }

            if (!this->how_to_fix.empty()) {
                out << std::endl << "how to fix: " << this->how_to_fix << std::endl;
            }

            if (!this->warning_message.empty()) {
                out << std::endl << "warning: " << this->warning_message << std::endl;
            }

            out << std::endl << std::endl;
        }
    }
};

// интерфейс проверки
class OwaspCheck {
protected:
    std::string name;
    std::string description;
    std::string how_to_fix;
    std::string warning_message;
public:
    std::string get_name() {
        return this->name;
    }
    virtual bool isApplicableTo(File&) = 0;
    virtual CheckResult check(File&) = 0;
};

#endif //PARSER_OWASPCHECK_H
