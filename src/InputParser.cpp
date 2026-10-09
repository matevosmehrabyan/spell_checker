#include <sstream>
#include <algorithm>
#include <cctype>

#include "InputParser.h"
#include "utils.h"


bool InputParser::parse_dictionary(std::istream& input, Dictionary& dest) {
    std::string line;

    while (std::getline(input, line)) {
        std::stringstream ss(line);
        std::string cur_word;

        while (ss >> cur_word) {
            if (cur_word == section_separator) {
                if (line.size() != section_separator.size()) {
                    std::cerr << "Parsing failed! The line should contain "
                              << "only the section separator" << std::endl;
                    return false;
                }
                return true;

            } else if (cur_word.size() > max_word_size || !is_word(cur_word)) {
                std::cerr << "Parsing failed! The input contains invalid input word '"
                          << cur_word << "'" << std::endl;
                return false;
            }

            dest.add(std::move(cur_word));
        }

    }

    std::cerr << "Section separator was not found!" << std::endl;
    return false;
}

bool InputParser::parse_text(std::istream& input, std::vector<Chunk>& dest) {
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

        size_t chunk_start{0};
        bool chunk_is_ws = std::isspace(static_cast<unsigned char>(line[0]));

        auto append_chunk = [&](std::string& str, bool is_ws) {
            if (!is_ws && (str.size() > max_word_size || !is_word(str))) {
                    std::cerr << "Invalid input word '" << str << "'" << std::endl;
                    return false;
                }

                dest.emplace_back(std::move(str), is_ws ? ChunkType::WS : ChunkType::WORD);
                return true;
        };

        for (size_t i = 0; i < line.size(); ++i) {
            bool cur_is_ws = std::isspace(static_cast<unsigned char>(line[i]));
            if (cur_is_ws != chunk_is_ws) {

                auto chunk = line.substr(chunk_start, i - chunk_start);
                if (chunk == section_separator) {
                    std::cerr << "Parsing failed! The line should contain "
                              << "only the section separator" << std::endl;
                    return false;
                }

                if (!append_chunk(chunk, chunk_is_ws)) {
                    return false;
                }

                chunk_start = i;
                chunk_is_ws = cur_is_ws;
            }

            if (i == line.size() - 1) {
                auto chunk = line.substr(chunk_start);
                if (chunk == section_separator) {
                    if (line.size() != section_separator.size()) {
                        std::cerr << "Parsing failed! The line should contain "
                                  << "only the section separator" << std::endl;
                        return false;
                    }
                    
                    return true;
                }

                if (!append_chunk(chunk, chunk_is_ws)) {
                    return false;
                }

            }
        }
    }

    std::cerr << "Section separator was not found!" << std::endl;
    return false;
}


bool InputParser::parse(std::istream& input) {
    Dictionary dict_words;
    if (!parse_dictionary(input, dict_words)) {
        return false;
    }

    std::vector<Chunk> parsed_text_lines;
    if (!parse_text(input, parsed_text_lines)) {
        return false;
    }

    dictionary = std::move(dict_words);
    text_lines = std::move(parsed_text_lines);

    return true;
}


const Dictionary& InputParser::get_dictionary() const {
    return dictionary;
}


const std::vector<Chunk>& InputParser::get_text() const {
    return text_lines;
}