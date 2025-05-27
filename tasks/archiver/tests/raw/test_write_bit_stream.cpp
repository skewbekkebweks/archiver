#include "../../src/write_bit_stream.cpp"

#include <sstream>
#include <iostream>

int main() {
    const std::string red = "\033[31m";
    const std::string green = "\033[32m";
    const std::string reset = "\033[0m";
    const std::string passed = "PASSED";
    const std::string failed = "FAILED";

    std::cout << "WriteBitStream Test" << std::endl;

    std::cout << "Test 1: Different types of writing" << std::endl;

    {
        std::stringstream ss;
        {
            WriteBitStream bs{ss};

            bs.WriteSeq("00000101");

            bs.WriteBits(8, 255);

            bs.WriteSeq("0011");
            bs.WriteBits(4, 12);

            bs.WriteSeq("0011");
        }

        unsigned char ch = 0;

        ss >> ch;
        if (ch == 5) {
            std::cout << green << passed << reset << std::endl;
        } else {
            std::cout << red << failed << " ch should be 5 but it is " << ch << reset << std::endl;
        }

        ss >> ch;
        if (ch == 255) {
            std::cout << green << passed << reset << std::endl;
        } else {
            std::cout << red << failed << " ch should be 255 but it is " << ch << reset << std::endl;
        }

        ss >> ch;
        if (ch == 60) {
            std::cout << green << passed << reset << std::endl;
        } else {
            std::cout << red << failed << " ch should be 60 but it is " << ch << reset << std::endl;
        }

        ss >> ch;
        if (ch == 48) {
            std::cout << green << passed << reset << std::endl;
        } else {
            std::cout << red << failed << " ch should be 48 but it is " << ch << reset << std::endl;
        }
    }
}
