#include "../src/read_bit_stream.cpp"

#include <sstream>
#include <iostream>

#include <bit>

int main() {
    const std::string red = "\033[31m";
    const std::string green = "\033[32m";
    const std::string reset = "\033[0m";
    const std::string passed = "PASSED";
    const std::string failed = "FAILED";

    std::cout << "ReadBitStream Test" << std::endl;

    std::cout << "Test 1: Different types of reading" << std::endl;

    std::stringstream ss;
    ss << static_cast<char>(212) << static_cast<char>(100) << static_cast<char>(1);

    ReadBitStream bs{ss};

    uint16_t i = 0;

    bs.ReadBits(8, i);
    if (i == 212) {
        std::cout << green << passed << reset << std::endl;
    } else {
        std::cout << red << failed << " i should be 212 but it is " << i << reset << std::endl;
    }

    bs.ReadBits(4, i);
    if (i == 6) {
        std::cout << green << passed << reset << std::endl;
    } else {
        std::cout << red << failed << " i should be 6 but it is " << i << reset << std::endl;
    }

    bs.ReadBits(2, i);
    if (i == 1) {
        std::cout << green << passed << reset << std::endl;
    } else {
        std::cout << red << failed << " i should be 1 but it is " << i << reset << std::endl;
    }

    bs.ReadBits(2, i);
    if (i == 0) {
        std::cout << green << passed << reset << std::endl;
    } else {
        std::cout << red << failed << " i should be 0 but it is " << i << reset << std::endl;
    }

    bs.ReadBits(8, i);
    if (i == 1) {
        std::cout << green << passed << reset << std::endl;
    } else {
        std::cout << red << failed << " i should be 1 but it is " << i << reset << std::endl;
    }

}
