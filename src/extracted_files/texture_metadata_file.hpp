#pragma once

#include "extracted_file.hpp"

#include <string>
#include <cstdint>
#include <vector>

class BinaryReaderFile;

struct TextureMetaData
{
    static const uint32_t identifier = 0xE977D350;
    uint32_t hash;
    uint32_t length;
    uint32_t hash2;

    uint32_t padding;
    uint32_t unknown;

    uint16_t width;
    uint16_t height;

    uint16_t padding2;
    uint8_t unknown2;

    uint8_t mipmap_level;

    uint32_t unknown3;
    uint32_t unknown4;
    uint32_t unknown5;
    uint32_t unknown6;
    uint32_t unknown7;

    uint32_t compression_format;
};

class TextureMetaDataFile : public ExtractedFile
{
public:
    TextureMetaDataFile(std::string file_path = "");
    ~TextureMetaDataFile() = default;

    void Parse();

    std::vector<TextureMetaData> texture_metadata;
    std::vector<uint8_t> other_data;

private:    
    void ParseMetaData(BinaryReaderFile &reader);
};
