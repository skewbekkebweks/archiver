#include "min_divisor.h"

int64_t MinDivisor(int64_t number) {
    for (int64_t divisor = 2; divisor * divisor <= number; ++divisor) {
        if (number % divisor == 0) {
            return divisor;
        }
    }

    return number;
}
