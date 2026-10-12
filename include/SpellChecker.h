#ifndef SPELL_CHECKER_H
#define SPELL_CHECKER_H

#include <cstdint>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <string_view>
#include <optional>

#include "Chunk.h"
#include "Dictionary.h"


class SpellChecker {
public:
    std::string spell_check(const Dictionary& dictionary, const std::vector<Chunk>& text_lines) const;

private:
    using WordCache = std::unordered_map<std::string, std::vector<std::string_view>>;

    enum class State {
        MATCH,
        INSERT,
        DELETE
    };

    struct StateInfo;

    std::optional<std::uint8_t> get_min_edit_count(const std::string& text_word,
                                                   const std::string& dict_word) const;

    std::vector<std::string_view> get_corrections(const std::string& word,
                                                  const Dictionary& dictionary) const;
};


struct SpellChecker::StateInfo {
    std::array<std::uint8_t, static_cast<uint8_t>(State::DELETE) + 1> info;
    uint8_t& operator[](State st);
    uint8_t get_min() const;
};


#endif