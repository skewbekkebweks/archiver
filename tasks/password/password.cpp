#include <cctype>
#include "password.h"

bool ValidatePassword(const std::string& password) {
    const char min_ascii_code = 33;
    const char max_ascii_code = 126;
    const size_t min_password_length = 7;
    const size_t max_password_length = 14;

    if (password.size() < min_password_length || password.size() > max_password_length) {
        return false;
    }

    for (char c : password) {
        if (c < min_ascii_code || c > max_ascii_code) {
            return false;
        }
    }

    int has_upper_letter = 0;
    int has_lower_letter = 0;
    int has_digit = 0;
    int has_other = 0;

    for (char c : password) {
        if (std::isupper(c)) {
            has_upper_letter = 1;
        }
        if (std::islower(c)) {
            has_lower_letter = 1;
        }
        if (std::isdigit(c)) {
            has_digit = 1;
        }
        if (std::ispunct(c)) {
            has_other = 1;
        }
    }

    if (has_upper_letter + has_lower_letter + has_digit + has_other >= 3) {
        return true;
    }

    return false;
}
