//
// Created by nafanya on 4/20/23.
//

#ifndef PARSER_UTILS_H
#define PARSER_UTILS_H

#include <string>

int count_segments(const char * path);
std::string last_path_segment(const char * path);
void normalize_spaces(std::string& s);

#endif //PARSER_UTILS_H
