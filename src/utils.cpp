#include <cctype>
#include <algorithm>

#include "utils.h"


bool is_word(const std::string& word) {
    return !word.empty() && std::all_of(word.begin(), word.end(), [](unsigned char c) {
                return std::isalpha(c);
            });
}

std::string to_lowercase(const std::string& txt) {
    auto lowered = txt;
    std::transform(lowered.begin(), lowered.end(), lowered.begin(),
                   [](unsigned char c) { return tolower(c); });
    return lowered;
}