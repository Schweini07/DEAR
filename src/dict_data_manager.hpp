#pragma once

#include "file_formats/dict.hpp"
#include "file_formats/data.hpp"
#include "extracted_files/file_table.hpp"
#include "extracted_files/mixed_data_file.hpp"
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
    void ExtractMixedData();
    void ExtractDataChildren(const FileData &file_data, const std::string &file_table_path);

    void RepackFile(std::vector<uint8_t> &data_file_data, FileSection &file_section);

    std::vector<uint8_t> CompressDataBuffer(const std::vector<uint8_t> &decompressed_buffer, uint32_t compressed_size);
    std::vector<uint8_t> DecompressDataBuffer(const std::vector<uint8_t> &compressed_buffer, uint32_t decompressed_size);

    std::unique_ptr<Dict> dict;
    std::unique_ptr<Data> data;
    std::unique_ptr<FileTable> file_table;
    std::unique_ptr<MixedDataFile> mixed_data_file;

    bool is_compressed;

    std::string destination_directory_path;
};
