#pragma once

#include <string>
#include <cstdint>
#include <vector>

class BinaryReaderFile;

struct TextureMetaData
{
    static const uint32_t identifier = 0xE977D350;
    uint32_t hash;
    uint32_t length;
    uint16_t width;
    uint16_t height;
    uint8_t mipmap_level;
    uint32_t compression_format;
};

class TextureMetaDataFile
{
public:
    TextureMetaDataFile(std::string file_path = "");
    ~TextureMetaDataFile() = default;

    void Parse();

    std::vector<TextureMetaData> texture_metadata;

private:    
    void ParseMetaData(BinaryReaderFile &reader);

private:
    std::string file_path;
};
