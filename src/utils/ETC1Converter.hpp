#pragma once

#include <cstdint>
#include <vector>

class ETC1Converter
{
public:
    ETC1Converter();
    ~ETC1Converter() = default;

    void Decode(std::vector<uint8_t> compressed_data, std::vector<uint8_t> &decompressed_data, uint16_t width, uint16_t height, bool alpha);
};
