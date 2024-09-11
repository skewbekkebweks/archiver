#include "next_even.h"

int64_t NextEven(int64_t n) {
    return n + 1 + (n + 1) % 2;
}
