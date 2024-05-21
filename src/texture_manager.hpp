#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "file_formats/texture.hpp"

class FileTable;
class FileData;
class TextureMetaDataFile;
class TextureMetaData;

struct TextureHeader
{

};

struct TextureFile
{
    TextureHeader header;
    std::vector<uint8_t> data;
};

class TextureManager
{
public:
    TextureManager(FileTable &file_table, TextureMetaDataFile &texture_metadata_file);
    ~TextureManager() = default;

    void ExtractTextures(std::string file_path);
    void ParseTextureHeader();

private:
    void ParseFileEntries();

    FileTable *file_table;
    std::vector<TextureMetaData> *texture_metadata_files;
    
    std::vector<Texture> textures;
};
