#pragma once

#include <map>
#include <vector>
#include <string>
#include <exception>

class ArgsParser {
public:
    ArgsParser(int argc, char* argv[]);

    bool HasArg(const std::string &arg) const;

    std::vector<std::string> GetArgValues(const std::string &arg) const;

    std::vector<std::string> GetPositionalArgs() const;

private:
    std::map<std::string, std::vector<std::string>> parsed_args_;
    std::vector<std::string> positional_args_;
};

class InvalidCommandLineArgumentsError : public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid command line arguments passed";
    }
};