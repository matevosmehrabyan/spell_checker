#include <vector>
#include <string>
#include <array>
#include <ranges>

#include "SpellChecker.h"
#include "Constraints.h"
#include "utils.h"


std::optional<std::uint8_t> SpellChecker::get_min_edit_count(const std::string& text_word,
                                                             const std::string& dict_word) const {

    constexpr std::uint8_t unreachable = constraints::max_allowed_edits + 1;

    std::vector<std::vector<StateInfo>>
    edit_info(text_word.size() + 1, std::vector<StateInfo>(dict_word.size() + 1,
                                                           StateInfo{{unreachable,
                                                                      unreachable,
                                                                      unreachable
                                                                    }}));
    edit_info[0][0][State::MATCH] = 0;

    constexpr std::array<State, 3> states{State::MATCH, State::INSERT, State::DELETE};

    for (std::size_t i = 0; i <= text_word.size(); ++i) {
        for (std::size_t j = 0; j <= dict_word.size(); ++j) {
            for(auto prev : states) {
                std::uint8_t edit_count = edit_info[i][j][prev];

                if (edit_count == unreachable) {
                    continue;
                }

                if (i < text_word.size() && j < dict_word.size() && text_word[i] == dict_word[j]) {
                    std::uint8_t cheap = std::min(edit_info[i + 1][j + 1][State::MATCH], edit_count);
                    edit_info[i + 1][j + 1][State::MATCH] = cheap;
                }

                if (i < text_word.size() && prev != State::DELETE) {
                    std::uint8_t cheap = std::min(edit_info[i + 1][j][State::DELETE],
                                                  static_cast<uint8_t>(edit_count + 1));
                    edit_info[i + 1][j][State::DELETE] = cheap;
                }

                if (j < dict_word.size() && prev != State::INSERT) {
                    std::uint8_t cheap = std::min(edit_info[i][j + 1][State::INSERT],
                                                  static_cast<uint8_t>(edit_count + 1));
                    edit_info[i][j + 1][State::INSERT] = cheap;
                }
            }
        }
    }

    std::uint8_t min_edits = edit_info[text_word.size()][dict_word.size()].get_min();
    if (min_edits <= constraints::max_allowed_edits) {
        return min_edits;
    }
    return std::nullopt;
}


std::vector<std::string_view> SpellChecker::get_corrections(const std::string& word,
                                                            const Dictionary& dictionary) const {
    std::vector<std::string_view> corrections;
    auto word_size = word.size();

    std::array<int8_t, 26> letter_map{};
    for (auto c : word) {
        ++letter_map[c - 'a'];
    }

    auto is_good_candidate = [&letter_map, &word_size](const std::string& candidate) {
        auto candidate_word_size = candidate.size();
        auto diff = (word_size > candidate_word_size) ? (word_size - candidate_word_size)
                                                      : (candidate_word_size - word_size);
        if (diff > constraints::max_allowed_edits) {
            return false;
        }

        auto cpy_letter_map = letter_map;
        for (auto c : candidate) {
            --cpy_letter_map[c - 'a'];
        }

        int total_diff = 0;
        for (int8_t letter_count : cpy_letter_map) {
            total_diff += std::abs(letter_count);
        }
        return total_diff <= constraints::max_allowed_edits;
    };

    uint8_t cheapest_edit = constraints::max_allowed_edits;
    for (const auto& dict_word : dictionary) {
        if (!is_good_candidate(dict_word)) {
            continue;
        }

        auto edits_count = get_min_edit_count(word, dict_word);
        if (!edits_count) {
            continue;
        }

        if (*edits_count < cheapest_edit) {
            corrections.clear();
            cheapest_edit = *edits_count;
        }

        if (*edits_count == cheapest_edit) {
            corrections.push_back(dictionary.get(dict_word));
        }
    }

    return corrections;
}


std::string SpellChecker::spell_check(const Dictionary& dictionary,
                                      const std::vector<Chunk>& text_lines) const {
    std::string final_output;
    WordCache cache;

    for (auto& chunk : text_lines) {
        if (chunk.type == ChunkType::WS) {
            final_output += chunk.content;
            continue;
        }

        auto lowered_word = to_lowercase(chunk.content);
        if (dictionary.contains(lowered_word)) {
            final_output += chunk.content;
            continue;
        }

        if (!cache.contains(lowered_word)) {
            cache.emplace(lowered_word, get_corrections(lowered_word, dictionary));
        }

        const auto& corrections = cache[lowered_word];
        if (corrections.empty()) {
            final_output += "{" + chunk.content + "?}";
            continue;
        }

        if (corrections.size() > 1) {
            final_output += "{";
            for (std::size_t i = 0; i < corrections.size(); ++i) {
                final_output += corrections[i];
                if (i != corrections.size() - 1) {
                    final_output += " ";
                }
            }
            final_output += "}";

            continue;
        }
        
        final_output += corrections[0];
    }

    return final_output;
}

uint8_t& SpellChecker::StateInfo::operator[](State st) {
    return info[static_cast<size_t>(st)];
}

uint8_t SpellChecker::StateInfo::get_min() const {
    return std::ranges::min(info);
}