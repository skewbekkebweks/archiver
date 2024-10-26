#include "../../src/args_parser.cpp"

#include <iostream>
#include <exception>
#include <string>
#include <vector>

int main() {
    const std::string red = "\033[31m";
    const std::string green = "\033[32m";
    const std::string reset = "\033[0m";
    const std::string passed = "PASSED";
    const std::string failed = "FAILED";

    std::cout << "ArgsParser test" << std::endl;

    {
        char* argv[] = {const_cast<char *>("program")};
        int argc = 1;
        ArgsParser parser{argc, argv};
        std::cout << "Test 1: No Arguments" << std::endl;
        if (!parser.HasArg("test")) {
            std::cout << green << "PASSED" << reset << std::endl;
        } else {
            std::cout << red << "FAILED" << " ArgsParser has an argument, although shouldn't" << reset << std::endl;
        }
    }

    {
        char* argv[] = {const_cast<char *>("program")};
        int argc = 1;
        ArgsParser parser{argc, argv};
        std::cout << "Test 2: Try to get non-existent argument" << std::endl;
        try {
            parser.GetArgValues("test");
            std::cout << red << "FAILED" << " ArgsParser has an argument, although shouldn't" << reset << std::endl;
        } catch (std::exception& ex) {
            std::cout << green << "PASSED" << reset << std::endl;
        }
    }

    {
        char* argv[] = {const_cast<char *>("program"), const_cast<char *>("first"), const_cast<char *>("second")};
        int argc = 3;
        ArgsParser parser{argc, argv};
        std::cout << "Test 3: Get positional arguments" << std::endl;
        std::vector<std::string> positional = parser.GetPositionalArgs();
        std::vector<std::string> positional_need = {"first", "second"};
        if (positional == positional_need) {
            std::cout << green << "PASSED" << reset << std::endl;
        } else {
            std::cout << red << "FAILED" << " Incorrect positional arguments" << reset << std::endl;
        }
    }

    {
        char* argv[] = {const_cast<char *>("program"), const_cast<char *>("first"), const_cast<char *>("-v"), const_cast<char *>("second"), const_cast<char *>("third")};
        int argc = 5;
        ArgsParser parser{argc, argv};
        std::cout << "Test 4: Check named argument" << std::endl;
        if (parser.HasArg("-v")) {
            std::vector<std::string> args = parser.GetArgValues("-v");
            std::vector<std::string> args_need = {"second", "third"};
            if (args == args_need) {
                std::cout << green << "PASSED" << reset << std::endl;
            } else {
                std::cout << red << "FAILED" << " Argument values are incorrect" << reset << std::endl;
            }
        } else {
            std::cout << red << "FAILED" << " Parser hasn't named argument" << reset << std::endl;
        }
    }
}