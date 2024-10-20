#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <string>

class ArgsParser {
public:
    ArgsParser(int argc, char* argv[]) {
        for (int i = 1; i < argc; ++i) {
            std::string current_arg = argv[i];

            if (current_arg.starts_with("-")) {
                std::string key = current_arg;
                std::vector<std::string> values;

                while (i + 1 < argc && !std::string(argv[i + 1]).starts_with("-")) {
                    values.push_back(argv[i + 1]);
                    ++i;
                }

                parsed_args_[key] = values;
            } else {
                positional_args_.push_back(current_arg);
            }
        }
    }

    bool HasArg(const std::string &arg) const {
        return parsed_args_.contains(arg);
    }

    std::vector<std::string> GetArgValues(const std::string &arg) const {
        return parsed_args_.at(arg);
    }

    std::vector<std::string> GetPositionalArgs() const {
        return positional_args_;
    }

private:
    std::map<std::string, std::vector<std::string>> parsed_args_;
    std::vector<std::string> positional_args_;
};