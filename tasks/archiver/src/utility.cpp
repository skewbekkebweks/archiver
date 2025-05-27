#include "utility.h"

void IncrementBinaryString(std::string& bin_str) {
    for (size_t size_minus_i = 1; size_minus_i <= bin_str.size(); ++size_minus_i) {
        size_t i = bin_str.size() - size_minus_i;
        if (bin_str[i] == '0') {
            bin_str[i] = '1';
            return;
        } else {
            bin_str[i] = '0';
        }
    }

    bin_str = "1" + bin_str;
}