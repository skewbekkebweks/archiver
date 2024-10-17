#include "provinces.h"
#include <set>
#include <cstdint>

int64_t PopMin(std::multiset<int64_t>& cnt) {
    int64_t value = *cnt.begin();
    cnt.erase(cnt.begin());
    return value;
}

int64_t CountPassports(const std::vector<int>& provinces) {
    std::multiset<int64_t> cnt;
    int64_t result = 0;
    for (int size : provinces) {
        cnt.insert(size);
    }

    while (cnt.size() > 1) {
        int64_t new_province_size = PopMin(cnt) + PopMin(cnt);
        result += new_province_size;
        cnt.insert(new_province_size);
    }

    return result;
}
