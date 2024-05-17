#include "dict_data_manager.hpp"

#include "utils/FileSystemDirectoryHandler.hpp"
#include <fstream>
#include <algorithm>
#include <zlib.h>

DictDataManager::DictDataManager(std::string dict_path, std::string data_path, std::string destination_directory_path)
: destination_directory_path(destination_directory_path)
{
    dict = std::make_unique<Dict>(dict_path);
    data = std::make_unique<Data>(data_path);
}

void DictDataManager::ExtractFiles()
{
    ParseDict();

    for (FileSection &file_section : dict->file_array)
        ExtractDataBufferToFile(file_section);
}

void DictDataManager::RepackFiles()
{
    std::vector<uint8_t> data_file_data;
    data_file_data.resize(data->GetFileSize());

    for (const FileSection &file_section : dict->file_array)
        RepackFile(data_file_data, file_section);
    
    std::ofstream repacked_file(destination_directory_path + "repacked.data", std::ios::binary);
    repacked_file.write(reinterpret_cast<char *>(data_file_data.data()), data_file_data.size());
    repacked_file.close();
}

void DictDataManager::ParseDict()
{
    dict->Parse();
    
    is_compressed = dict->header.compression_flag;
}

void DictDataManager::ExtractDataBufferToFile(FileSection &file_section)
{
    std::vector<uint8_t> data_buffer;
    uint32_t data_buffer_size = is_compressed ? file_section.compressed_file_length : file_section.decompressed_file_length;
    
    data->GetDataBuffer(data_buffer, file_section.offset, data_buffer_size);

    if (is_compressed)
        data_buffer = DecompressDataBuffer(data_buffer, file_section.decompressed_file_length);

    FileSystemDirectoryHandler dir_handler;
    dir_handler.CreateDirectory(destination_directory_path.c_str());
    file_section.file_path = destination_directory_path + "file" + std::to_string(file_section.id);

    std::ofstream extracted_file(file_section.file_path, std::ios::binary);
    extracted_file.write(reinterpret_cast<char *>(data_buffer.data()), file_section.decompressed_file_length);
    extracted_file.close();
}

void DictDataManager::RepackFile(std::vector<uint8_t> &data_file_data, const FileSection &file_section)
{
    if (0 == file_section.decompressed_file_length)
        return;

    // 0x1 is a debug file, which we do not want to repack
    if (0x1 == file_section.file_extension)
        return;

    std::vector<uint8_t> file_data;
    file_data.resize(file_section.decompressed_file_length);

    std::ifstream extracted_file(file_section.file_path, std::ios::binary);
    extracted_file.read(reinterpret_cast<char *>(file_data.data()), file_section.decompressed_file_length);
    extracted_file.close();

    if (is_compressed)
        file_data = CompressDataBuffer(file_data, file_section.compressed_file_length);
    
    std::copy(file_data.begin(), file_data.end(), data_file_data.begin()+file_section.offset);
}

std::vector<uint8_t> DictDataManager::CompressDataBuffer(const std::vector<uint8_t> &decompressed_buffer, uint32_t compressed_size)
{
    std::vector<uint8_t> compressed_buffer;
    compressed_buffer.resize(compressed_size);

    compress(
        compressed_buffer.data(),
        reinterpret_cast<uLongf *>(&compressed_size),
        decompressed_buffer.data(),
        decompressed_buffer.size()
    );

    return compressed_buffer;
}

std::vector<uint8_t> DictDataManager::DecompressDataBuffer(const std::vector<uint8_t> &compressed_buffer, uint32_t decompressed_size)
{
    std::vector<uint8_t> decompressed_buffer;
    decompressed_buffer.resize(decompressed_size);

    uncompress(
        decompressed_buffer.data(),
        reinterpret_cast<uLongf *>(&decompressed_size),
        compressed_buffer.data(),
        compressed_buffer.size()
    );

    return decompressed_buffer;
}
