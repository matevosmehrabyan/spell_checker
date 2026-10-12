#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H

#include <iostream>
#include <vector>
#include <string>
#include <string_view>

#include "Chunk.h"
#include "Dictionary.h"


class InputParser {
public:
    void parse(std::istream& input);
    const Dictionary& get_dictionary() const;
    const std::vector<Chunk>& get_text() const;

private:
    static constexpr std::string_view section_separator = "===";
    static constexpr std::size_t max_word_size = 50;

    Dictionary dictionary;
    std::vector<Chunk> text_lines;

    void validate_word(const std::string& word) const;

    void parse_dictionary(std::istream& input, Dictionary& dest) const;
    void parse_text(std::istream& input, std::vector<Chunk>& dest) const;
};


#endif