#include <cctype>
#include <format>
#include <sstream>
#include <algorithm>
#include <stdexcept>

#include "InputParser.h"
#include "ErrorMessages.h"
#include "utils.h"


void InputParser::validate_word(const std::string& word) const {
    if (word.size() > max_word_size) {
        throw std::runtime_error(std::format("{}: '{}'", err_msg::large_word, word));
    } else if (!is_word(word)) {
        throw std::runtime_error(std::format("{}: '{}'", err_msg::invalid_word, word));
    }
}

void InputParser::parse_dictionary(std::istream& input, Dictionary& dest) const {
    std::string line;

    while (std::getline(input, line)) {
        std::stringstream ss(line);
        std::string cur_word;

        while (ss >> cur_word) {
            if (cur_word == section_separator) {
                if (line.size() != section_separator.size()) {
                    throw std::runtime_error(err_msg::invalid_separator);
                }
                return;

            }

            validate_word(cur_word);
            dest.add(std::move(cur_word));
        }
    }

    throw std::runtime_error(err_msg::separator_not_found);
}

void InputParser::parse_text(std::istream& input, std::vector<Chunk>& dest) const {
    std::string line;
    bool is_first_line{true};

    while (std::getline(input, line)) {

        if (is_first_line) {
            is_first_line = false;
        } else {
            dest.emplace_back("\n", ChunkType::WS);
        }

        if (line.empty()) {
            continue;
        }

        std::size_t chunk_start{0};
        bool chunk_is_ws = std::isspace(static_cast<unsigned char>(line[0]));

        auto append_chunk = [&](std::string& str, bool is_ws) {
            if (!is_ws) {
                validate_word(str);
            }

            dest.emplace_back(std::move(str), is_ws ? ChunkType::WS : ChunkType::WORD);
        };

        for (std::size_t i = 0; i < line.size(); ++i) {
            bool cur_is_ws = std::isspace(static_cast<unsigned char>(line[i]));
            if (cur_is_ws != chunk_is_ws) {

                auto chunk = line.substr(chunk_start, i - chunk_start);
                if (chunk == section_separator) {
                    throw std::runtime_error(err_msg::invalid_separator);
                }

                append_chunk(chunk, chunk_is_ws);
                chunk_start = i;
                chunk_is_ws = cur_is_ws;
            }

            if (i == line.size() - 1) {
                auto chunk = line.substr(chunk_start);
                if (chunk == section_separator) {
                    if (line.size() != section_separator.size()) {
                        throw std::runtime_error(err_msg::invalid_separator);
                    }
                    
                    return;
                }

                append_chunk(chunk, chunk_is_ws);
            }
        }
    }

    throw std::runtime_error(err_msg::separator_not_found);
}


void InputParser::parse(std::istream& input) {
    Dictionary dict_words;
    parse_dictionary(input, dict_words);

    std::vector<Chunk> parsed_text_lines;
    parse_text(input, parsed_text_lines);

    dictionary = std::move(dict_words);
    text_lines = std::move(parsed_text_lines);
}


const Dictionary& InputParser::get_dictionary() const {
    return dictionary;
}


const std::vector<Chunk>& InputParser::get_text() const {
    return text_lines;
}