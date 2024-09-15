#include "reduce_fraction.h"

int64_t Gcd(int64_t a, int64_t b) {
    if (b == static_cast<int64_t>(0)) {
        return a;
    }
    if (a < b) {
        return Gcd(b, a);
    }
    return Gcd(b, a % b);
}

int64_t ReduceFraction(int64_t numerator, int64_t denominator) {
    int64_t gcd = Gcd(numerator, denominator);

    numerator /= gcd;
    denominator /= gcd;

    return numerator + denominator;
}
