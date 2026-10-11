#ifndef CHUNK_H
#define CHUNK_H

#include <string>


enum class ChunkType {
    WS,
    WORD
};


struct Chunk {
    std::string content;
    ChunkType type{ChunkType::WS};
};

#endif