#pragma once

#include "file_formats/dict.hpp"
#include "file_formats/data.hpp"
#include <string>
#include <memory>
#include <vector>

class DictDataManager
{
public:
    DictDataManager(std::string dict_path, std::string data_path, std::string destination_directory_path);
    ~DictDataManager() = default;

    void ExtractFiles();
    void RepackFiles();

private:
    void ParseDict();
    void ExtractDataBufferToFile(FileSection &file_section);
    void RepackFile(std::vector<uint8_t> &data_file_data, const FileSection &file_section);
    std::vector<uint8_t> CompressDataBuffer(const std::vector<uint8_t> &decompressed_buffer, uint32_t compressed_size);
    std::vector<uint8_t> DecompressDataBuffer(const std::vector<uint8_t> &compressed_buffer, uint32_t decompressed_size);

    std::unique_ptr<Dict> dict;
    std::unique_ptr<Data> data;

    bool is_compressed;

    std::string destination_directory_path;
};
