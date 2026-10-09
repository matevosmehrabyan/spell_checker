#ifndef SPELL_CHECKER_H
#define SPELL_CHECKER_H

#include <string>
#include <vector>

#include "Dictionary.h"


class SpellChecker {
    static constexpr size_t max_allowed_edits = 2;

    enum class State {
        MATCH,
        INSERT,
        DELETE
    };

    const Dictionary& dictionary;

    std::vector<std::string> get_corrections(const std::string& word, const Dictionary& dictionary);
    bool can_be_fixed(const std::string& word_1, const std::string& word_2, size_t allowed_edits);
    bool edit(const std::string& w1, size_t pos1, const std::string& w2, size_t pos2,
              State prev, size_t edits_allowed);

public:
    explicit SpellChecker(const Dictionary& dict): dictionary(dict){}
    
    SpellChecker(const SpellChecker&) = default;
    SpellChecker& operator=(const SpellChecker&) = default;

    SpellChecker(SpellChecker&&) = default;
    SpellChecker& operator=(SpellChecker&&) = default;

    std::string spell_check(const std::vector<Chunk>& text_lines);
};


#endif