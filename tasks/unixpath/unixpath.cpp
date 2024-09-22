#include "unixpath.h"
#include <string>
#include <string_view>
#include <vector>
#include <sstream>
#include <iostream>

std::vector<std::string_view> Split(std::string_view str) {
    std::vector<std::string_view> result;
    size_t start = 0;
    size_t end = 0;
    while ((end = str.find('/', start)) != std::string_view::npos) {
        result.emplace_back(str.substr(start, end - start));
        start = end + 1;
    }

    result.emplace_back(str.substr(start, str.size() - start));

    return result;
}

std::string NormalizePath(std::string_view current_working_dir, std::string_view path) {
    std::vector<std::string_view> working_dir;
    std::vector<std::string_view> paths;

    if (path[0] != '/') {
        std::vector<std::string_view> split_current_working_dir = Split(current_working_dir);
        paths.insert(paths.end(), split_current_working_dir.begin(), split_current_working_dir.end());
    }
    std::vector<std::string_view> split_path = Split(path);
    paths.insert(paths.end(), split_path.begin(), split_path.end());

    for (const std::string_view& jump : paths) {
        if (jump.empty() || jump == ".") {
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

    std::string response;
    for (const std::string_view& part : working_dir) {
        response += "/" + std::string(part);
    }

    if (working_dir.empty()) {
        response += "/";
    }

    return response;
}
