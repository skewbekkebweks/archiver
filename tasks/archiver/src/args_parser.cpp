#include "args_parser.h"

ArgsParser::ArgsParser(int argc, char *argv[]) {
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

bool ArgsParser::HasArg(const std::string &arg) const {
    return parsed_args_.contains(arg);
}

std::vector<std::string> ArgsParser::GetArgValues(const std::string &arg) const {
    return parsed_args_.at(arg);
}

std::vector<std::string> ArgsParser::GetPositionalArgs() const {
    return positional_args_;
}

bool ArgsParser::ValidateArg(const std::string &arg, size_t count, BoundType bound_type) const {
    if (!HasArg(arg)) {
        return false;
    }

    switch (bound_type) {
        case at_least:
            return count <= GetArgValues(arg).size();
            break;
        case exact:
            return count == GetArgValues(arg).size();
            break;
        case as_maximum:
            return count >= GetArgValues(arg).size();
            break;
    }

    return false;
}