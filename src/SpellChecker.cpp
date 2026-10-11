#include <vector>
#include <string>

#include "SpellChecker.h"
#include "utils.h"


bool SpellChecker::can_transform_from(const std::string& text_word, std::size_t pos1,
                                      const std::string& dict_word, std::size_t pos2,
                                      State prev, std::size_t edits_allowed) {

    while (pos1 < text_word.size() && pos2 < dict_word.size()) {
        if (text_word[pos1] != dict_word[pos2]) {
            break;
        }

        ++pos1;
        ++pos2;
        prev = State::MATCH;
    }

    if (pos1 == text_word.size() && pos2 == dict_word.size()) {
        return true;
    }

    if (edits_allowed == 0) {
        return false;
    }

    if (pos1 < text_word.size() && prev != State::DELETE
        && can_transform_from(text_word, pos1 + 1, dict_word, pos2,
                              State::DELETE, edits_allowed - 1)) {

        return true;
    }

    if (pos2 < dict_word.size() && prev != State::INSERT
        && can_transform_from(text_word, pos1, dict_word, pos2 + 1,
                              State::INSERT, edits_allowed - 1)) {

        return true;
    }
    
    return false;
}


bool SpellChecker::is_transformable(const std::string& text_word, const std::string& dict_word,
                                    std::size_t allowed_edits) {
    return can_transform_from(text_word, 0, dict_word, 0, State::MATCH, allowed_edits);
}


std::vector<std::string> SpellChecker::get_corrections(const std::string& word,
                                                       const Dictionary& dictionary) {
    std::vector<std::string> corrections;
    auto word_size = word.size();

    auto is_good_candidate = [&word_size](const std::string& candidate, std::size_t edit_count) {
        auto candidate_word_size = candidate.size();
        auto diff = (word_size > candidate_word_size) ? (word_size - candidate_word_size)
                                                      : (candidate_word_size - word_size);
        return diff <= edit_count;
    };

    for (std::size_t i = 1; i <= max_allowed_edits; ++i) { // could make one-pass, but code would get messy
        for (const auto& dict_word : dictionary) {
            if (is_good_candidate(dict_word, i) && is_transformable(word, dict_word, i)) {
                corrections.push_back(dictionary.get(dict_word));
            }
        }
        if (!corrections.empty()) {
            break;
        }
    }

    return corrections;
}


std::string SpellChecker::spell_check(const Dictionary& dictionary,
                                      const std::vector<Chunk>& text_lines) {
    std::string final_output;

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

        auto corrections = get_corrections(lowered_word, dictionary);
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