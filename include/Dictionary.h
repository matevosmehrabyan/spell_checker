#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <vector>
#include <string>
#include <unordered_map>


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


class Dictionary {
    std::vector<std::string> lowered_words;
    std::unordered_map<std::string, std::string> lowered_to_orig;

public:

    using const_iterator = std::vector<std::string>::const_iterator;
    const_iterator begin() const;
    const_iterator end() const;

    bool add(const std::string& elem);
    bool add(std::string&& elem);

    bool contains(const std::string& elem) const;
    const std::string& get(const std::string& elem) const;
};

#endif