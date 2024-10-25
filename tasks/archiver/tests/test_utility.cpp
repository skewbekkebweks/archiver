#include "../src/utility.cpp"

#include <iostream>

int main() {
    const std::string red = "\033[31m";
    const std::string green = "\033[32m";
    const std::string reset = "\033[0m";
    const std::string passed = "PASSED";
    const std::string failed = "FAILED";

    std::cout << "Utility test" << std::endl;

    std::cout << "Test 1: Incrementing a simple binary string" << std::endl;
    std::string test1 = "0001";
    IncrementBinaryString(test1);
    if (test1 == "0010") {
        std::cout << green << passed << reset << std::endl;
    } else {
        std::cout << red << failed << " result = " << test1 << " but should 0010" << reset << std::endl;
    }
    
    std::cout << "Test 2: Incrementing a string with all 1's" << std::endl;
    std::string test2 = "1111";
    IncrementBinaryString(test2);
    if (test2 == "10000") {
        std::cout << green << passed << reset << std::endl;
    } else {
        std::cout << red << failed << " result = " << test2 << " but should 10000" << reset << std::endl;
    }
    
    std::cout << "Test 3: Incrementing a string with leading zeros" << std::endl;
    std::string test3 = "0000";
    IncrementBinaryString(test3);
    if (test3 == "0001") {
        std::cout << green << passed << reset << std::endl;
    } else {
        std::cout << red << failed << " result = " << test3 << " but should 0001" << reset << std::endl;
    }
    
    std::cout << "Test 4: Incrementing a string with alternating bits" << std::endl;
    std::string test4 = "1010";
    IncrementBinaryString(test4);
    if (test4 == "1011") {
        std::cout << green << passed << reset << std::endl;
    } else {
        std::cout << red << failed << " result = " << test4 << " but should 1011" << reset << std::endl;
    }
    
    std::cout << "Test 5: Incrementing a string that’s one less than the next power of two" << std::endl;
    std::string test5 = "0111";
    IncrementBinaryString(test5);
    if (test5 == "1000") {
        std::cout << green << passed << reset << std::endl;
    } else {
        std::cout << red << failed << " result = " << test5 << " but should 1000" << reset << std::endl;
    }

    std::cout << "Test 6: Incrementing an empty string" << std::endl;
    std::string test6;
    IncrementBinaryString(test6);
    if (test6 == "1") {
        std::cout << green << passed << reset << std::endl;
    } else {
        std::cout << red << failed << " result = " << test6 << " but should 1" << reset << std::endl;
    }

    std::cout << "Test 7: Incrementing a binary string with leading and trailing mixed bits" << std::endl;
    std::string test7 = "100010";
    IncrementBinaryString(test7);
    if (test7 == "100011") {
        std::cout << green << passed << reset << std::endl;
    } else {
        std::cout << red << failed << " result = " << test7 << " but should 100011" << reset << std::endl;
    }
}