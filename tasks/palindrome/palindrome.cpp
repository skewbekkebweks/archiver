#include "palindrome.h"

bool IsPalindrome(const std::string& str) {
    if (str.empty()) {
        return true;
    }

    size_t left_pointer = 0;
    size_t right_pointer = str.size() - 1;

    while (true) {
        while (left_pointer < str.size() && str[left_pointer] == ' ') {
            left_pointer++;
        }

        if (left_pointer == str.size()) {
            // all elements are checked
            return true;
        }

        while (str[right_pointer] == ' ') {  // if left_pointer < str.size() then right_pointer will be >= 0
            right_pointer--;
        }

        if (str[left_pointer] != str[right_pointer]) {
            return false;
        }

        if (left_pointer == str.size() - 1 || right_pointer == 0) {
            // all elemnets are checked
            return true;
        }

        left_pointer++;
        right_pointer--;
    }
    return true;
}
