#include "Dictionary.h"
#include "utils.h"


Dictionary::const_iterator Dictionary::begin() const {
    return lowered_words.cbegin();
}


Dictionary::const_iterator Dictionary::end() const  {
    return lowered_words.cend();
}


bool Dictionary::add(const std::string& elem) {
    auto lowered = to_lowercase(elem);

    if (lowered_to_orig.contains(lowered)) {
        return false;
    }

    lowered_to_orig[std::move(lowered)] = elem;
    lowered_words.push_back(elem);
    return true;
}


bool Dictionary::add(std::string&& elem) {
    auto lowered = to_lowercase(elem);

    if (lowered_to_orig.contains(lowered)) {
        return false;
    }

    lowered_to_orig[std::move(lowered)] = elem;
    lowered_words.push_back(std::move(elem));
    return true;
}


bool Dictionary::contains(const std::string& elem) const {
    return lowered_to_orig.contains(elem);
}


const std::string& Dictionary::get(const std::string& elem) const {
    return lowered_to_orig.at(elem);
}