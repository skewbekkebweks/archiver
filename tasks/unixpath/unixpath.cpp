#include "unixpath.h"
#include <vector>
#include <sstream>

std::vector<std::string> Split(std::string str) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string part;

    while (std::getline(ss, part, '/')) {
        if (!part.empty()) {
            result.push_back(part);
        }
    }

    return result;
}

std::string NormalizePath(std::string_view current_working_dir, std::string_view path) {
    std::vector<std::string> working_dir;
    std::vector<std::vector<std::string>> paths;

    if (path[0] != '/') {
        paths.push_back(Split(std::string(current_working_dir)));
    }
    paths.push_back(Split(std::string(path)));

    for (const std::vector<std::string>& path : paths) {
        for (const std::string& jump : path) {
            if (jump == ".") {
                continue;
            }
            if (jump == "..") {
                if (!working_dir.empty()) {
                    working_dir.pop_back();
                }
            } else {
                working_dir.push_back(jump);
            }
        }
    }

    std::string response;
    for (const std::string& part : working_dir) {
        response += "/" + part;
    }

    if (working_dir.empty()) {
        response += "/";
    }

    return response;
}
