#pragma once

#include <cstdint>
#include <vector>

class ETC1Converter
{
public:
    ETC1Converter();
    ~ETC1Converter() = default;

    void Decode(std::vector<char> compressed_data, std::vector<char> &decompressed_data, uint16_t width, uint16_t height, bool alpha);

private:
    void DecodeBlock(uint64_t *block);
};
