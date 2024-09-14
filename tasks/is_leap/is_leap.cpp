#include "is_leap.h"

bool IsLeap(int year) {
    const int four_hundreds = 400;
    const int hundred = 100;

    if (year % four_hundreds == 0) {
        return true;
    }

    if (year % hundred == 0) {
        return false;
    }

    if (year % 4 == 0) {
        return true;
    }

    return false;
}
