#include "utility.h"

std::string IncrementBinaryString(std::string bin_str) {
    int n = static_cast<int>(bin_str.size());
    
    for (int i = n - 1; i >= 0; --i) {
        if (bin_str[i] == '0') {
            bin_str[i] = '1';
            return bin_str;
        } else {
            bin_str[i] = '0';
        }
    }

    bin_str = "1" + bin_str;
    
    return bin_str;
}