#include "fibonacci.h"

int64_t CalculateNthFibonacci(int64_t n) {
    int64_t fibonacci_n = 0;
    int64_t fibonacci_n_plus_1 = 1;

    for (int64_t i = 1; i <= n; ++i) {
        int64_t fibonacci_n_plus_2 = fibonacci_n + fibonacci_n_plus_1;
        fibonacci_n = fibonacci_n_plus_1;
        fibonacci_n_plus_1 = fibonacci_n_plus_2;
    }

    return fibonacci_n;
}
