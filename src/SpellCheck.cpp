#include <vector>
#include <string>

#include "InputParser.h"
#include "utils.h"


enum class State {
    MATCH,
    INSERT,
    DELETE
};


constexpr size_t max_allowed_edits = 2;


bool edit(const std::string& w1, size_t pos1, const std::string& w2, size_t pos2,
          State prev, size_t edits_allowed) {

    while (pos1 < w1.size() && pos2 < w2.size()) {
        if (w1[pos1] != w2[pos2]) {
            break;
        }

        ++pos1;
        ++pos2;
        prev = State::MATCH;
    }

    if (pos1 == w1.size() && pos2 == w2.size()) {
        return true;
    }

    if (edits_allowed == 0) {
        return false;
    }

    if (pos1 < w1.size() && prev != State::DELETE
        && edit(w1, pos1 + 1, w2, pos2, State::DELETE, edits_allowed - 1)) {

        return true;
    }

    if (pos2 < w2.size() && prev != State::INSERT
        && edit(w1, pos1, w2, pos2 + 1, State::INSERT, edits_allowed - 1)) {

        return true;
    }
    
    return false;
}


bool can_be_fixed(const std::string& word_1, const std::string& word_2, size_t allowed_edits) {
    return edit(word_1, 0, word_2, 0, State::MATCH, allowed_edits);
}


std::vector<std::string> get_corrections(const std::string& word,
                                         const Dictionary& dictionary) {
    std::vector<std::string> corrections;
    auto word_size = word.size();

    auto is_good_candidate = [&word_size](const std::string& candidate, size_t edit_count) {
        auto candidate_word_size = candidate.size();
        auto diff = (word_size > candidate_word_size) ? (word_size - candidate_word_size)
                                                      : (candidate_word_size - word_size);
        return diff <= edit_count;
    };

    for (size_t i = 1; i <= max_allowed_edits; ++i) { // could make one-pass, but code would get messy
        for (const auto& dict_word : dictionary) {
            if (is_good_candidate(dict_word, i) && can_be_fixed(word, dict_word, i)) {
                corrections.push_back(dictionary.get(dict_word));
            }
        }
        if (!corrections.empty()) {
            break;
        }
    }

    return corrections;
}


std::string spell_check(const Dictionary& dictionary, const std::vector<Chunk>& text_lines) {

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
            for (size_t i = 0; i < corrections.size(); ++i) {
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