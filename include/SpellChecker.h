#ifndef SPELL_CHECKER_H
#define SPELL_CHECKER_H

#include <string>
#include <vector>

#include "Chunk.h"
#include "Dictionary.h"


class SpellChecker {
public:
    std::string spell_check(const Dictionary& dictionary, const std::vector<Chunk>& text_lines) const;

private:
    static constexpr std::size_t max_allowed_edits = 2;

    enum class State {
        MATCH,
        INSERT,
        DELETE
    };

    std::vector<std::string> get_corrections(const std::string& word, const Dictionary& dictionary) const;
    bool is_transformable(const std::string& text_word, const std::string& dict_word,
                          std::size_t allowed_edits) const;
    bool can_transform_from(const std::string& text_word, std::size_t pos1, const std::string& dict_word,
                            std::size_t pos2, State prev, std::size_t edits_allowed) const;
};


#endif