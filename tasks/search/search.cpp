#include <algorithm>
#include <cctype>
#include <cmath>
#include "search.h"
#include <string_view>
#include <vector>

const double EPS = 1e-9;

bool TfIdfPairsCompare(std::pair<double, size_t> p1, std::pair<double, size_t> p2) {
    if (fabs(p1.first - p2.first) > EPS) {
        return p1.first > p2.first;
    }

    return p1.second < p2.second;
}

bool CaseInsensitiveCompare(std::string_view s1, std::string_view s2) {
    if (s1.size() != s2.size()) {
        return false;
    }

    return std::equal(s1.begin(), s1.end(), s2.begin(), s2.end(),
                      [](char a, char b) { return std::tolower(a) == std::tolower(b); });
}

std::vector<std::string_view> SplitByCondition(std::string_view str, auto condition) {
    std::vector<std::string_view> result;
    while (!str.empty()) {
        size_t end = 0;
        while (end < str.size() && !condition(str[end])) {
            end++;
        }

        if (end != 0) {
            result.push_back(str.substr(0, end));
        }
        str.remove_prefix(std::min(str.size(), end + 1));
    }

    return result;
}

std::vector<std::string_view> Search(std::string_view text, std::string_view query, size_t results_count) {
    std::vector<std::string_view> text_strings = SplitByCondition(text, [](char c) -> bool { return c == '\n'; });
    size_t count_text_strings = text_strings.size();

    std::vector<std::vector<std::string_view>> text_strings_words;
    for (size_t i = 0; i < count_text_strings; ++i) {
        std::vector<std::string_view> new_string_words =
            SplitByCondition(text_strings[i], [](char c) -> bool { return !std::isalpha(c); });

        if (!new_string_words.empty()) {
            text_strings_words.push_back(new_string_words);
        }
    }
    count_text_strings = text_strings_words.size();

    std::vector<std::string_view> query_words =
        SplitByCondition(query, [](char c) -> bool { return !std::isalpha(c); });
    size_t count_query_words = query_words.size();

    std::vector<int> idf(count_query_words, 0);

    for (size_t i = 0; i < count_query_words; ++i) {
        std::string_view query_word = query_words[i];
        for (const std::vector<std::string_view>& text_string : text_strings_words) {
            for (std::string_view string_word : text_string) {
                if (CaseInsensitiveCompare(query_word, string_word)) {
                    idf[i]++;
                    break;
                }
            }
        }
    }

    std::vector<std::pair<double, size_t>> tf_idf;

    for (size_t i = 0; i < count_text_strings; ++i) {
        double result = 0;
        for (size_t j = 0; j < count_query_words; ++j) {
            if (idf[j] == 0) {
                continue;
            }
            std::string_view query_word = query_words[j];
            int occurrences_number = 0;
            for (std::string_view string_word : text_strings_words[i]) {
                if (CaseInsensitiveCompare(query_word, string_word)) {
                    occurrences_number++;
                }
            }
            result += static_cast<double>(occurrences_number) / static_cast<double>(text_strings_words[i].size()) *
                      log(static_cast<double>(count_text_strings) / static_cast<double>(idf[j]));
        }
        if (result > EPS) {
            tf_idf.emplace_back(result, i);
        }
    }

    std::sort(tf_idf.begin(), tf_idf.end(), TfIdfPairsCompare);

    std::vector<std::string_view> result;

    for (size_t i = 0; i < std::min(results_count, tf_idf.size()); ++i) {
        result.push_back(text_strings[tf_idf[i].second]);
    }

    return result;
}
