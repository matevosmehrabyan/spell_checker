#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H

#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>

#include "Dictionary.h"


enum class ChunkType {
    WS,
    WORD
};


struct Chunk {
    std::string content;
    ChunkType type;

    Chunk(const std::string& content, ChunkType type): content(content), type(type) {}
    
    Chunk(const Chunk&) = default;
    Chunk& operator=(const Chunk&) = default;

    Chunk(Chunk&&) = default;
    Chunk& operator=(Chunk&&) = default;
};


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