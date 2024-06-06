#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

class Dict;
class FileSection;
class Data;
class FileTable;
class MixedDataFile;
class FileData;
class TextureMetaDataFile;
class TextureManager;

class DictDataExtractor
{
public:
    DictDataExtractor(Dict *dict, Data *data, std::string destination_directory_path);
    ~DictDataExtractor();

    void Extract();

    std::unique_ptr<FileTable> file_table;
    std::unique_ptr<MixedDataFile> mixed_data;
    std::unique_ptr<TextureMetaDataFile> texture_metadata;
    std::unique_ptr<TextureManager> texture_manager;

private:
    void ExtractFiles();
    void ExtractDataBufferToFile(FileSection &file_section);
    std::vector<uint8_t> DecompressDataBuffer(const std::vector<uint8_t> &compressed_buffer, uint32_t decompressed_size);

    void ExtractMixedData();
    void ExtractDataChildren(const FileData &file_data, const std::string &file_table_path);
    
    void ExtractTextures();

    Dict *dict;
    Data *data;
    std::string destination_directory_path;
};
