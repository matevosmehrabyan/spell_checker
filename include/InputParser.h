#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H

#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>

#include "Dictionary.h"


class InputParser {
    static constexpr std::string_view section_separator = "===";
    static constexpr std::size_t max_word_size = 50;

    Dictionary dictionary;
    std::vector<Chunk> text_lines;

    bool parse_dictionary(std::istream& input, Dictionary& dest);
    bool parse_text(std::istream& input, std::vector<Chunk>& dest);

public:
    bool parse(std::istream& input);
    const Dictionary& get_dictionary() const;
    const std::vector<Chunk>& get_text() const;
};


#endif