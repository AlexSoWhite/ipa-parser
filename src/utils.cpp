//
// Created by nafanya on 4/20/23.
//

#include <string>

int count_segments(const char * path) {
    std::string lp(path);
    int res = 0;
    while (lp.find('/') != std::string::npos) {
        lp = lp.substr(lp.find('/') + 1);
        res++;
    }
    return res;
}

std::string last_path_segment(const char * path) {
    std::string lp(path);
    lp = lp.substr(lp.find_last_of('/')+1);
    return lp;
}

void normalize_spaces(std::string& s) {
    int idx = 0;
    while (idx < s.size()) {
        if (s[idx] == ' ') {
            s = s.substr(0, idx) + "\\" + s.substr(idx);
            idx++;
        }
        idx++;
    }
}