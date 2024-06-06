#include "dict_data_manager.hpp"

#include <algorithm>
#include <iostream>

DictDataManager::DictDataManager(std::string dict_path, std::string data_path, std::string destination_directory_path)
: destination_directory_path(destination_directory_path)
{
    dict = std::make_unique<Dict>(dict_path);
    data = std::make_unique<Data>(data_path);

    dict->Parse();
}

void DictDataManager::Extract()
{
    extractor = std::make_unique<DictDataExtractor>(dict.get(), data.get(), destination_directory_path);
    extractor->Extract();
}

void DictDataManager::Repack()
{
    DictDataRepacker repacker(
        extractor->file_table.get(),
        extractor->texture_metadata.get(),
        extractor->mixed_data.get(),
        destination_directory_path
    );
    repacker.Repack();
}

/*
void DictDataManager::RepackFiles()
{
    std::vector<uint8_t> data_file_data;
    data_file_data.resize(data->GetFileSize());

    RepackMixedDataFile();
    RepackFileTable();
    RepackDataFile();
}

void DictDataManager::RepackFile(std::vector<uint8_t> &data_file_data, FileSection &file_section)
{
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

void DictDataManager::RepackDataFile()
{
    std::ofstream repacked_data_file(destination_directory_path + "data.repacked", std::ios::binary);

    std::ifstream file_table_file(file_table->GetFilePath(), std::ios::binary);
    repacked_data_file << file_table_file.rdbuf();
    file_table_file.close();

    std::ifstream texture_metadata_file(texture_metadata->GetFilePath(), std::ios::binary);
    repacked_data_file << texture_metadata_file.rdbuf();
    texture_metadata_file.close();

    std::ifstream mixed_data_file(mixed_data->GetFilePath(), std::ios::binary);
    repacked_data_file << mixed_data_file.rdbuf();
    mixed_data_file.close();

    repacked_data_file.close();
}

void DictDataManager::RepackFileTable()
{
    std::ofstream new_file_table(destination_directory_path + "file0.repacked", std::ios::binary);

    for (const FileEntry &file_entry : file_table->file_entries)
    {
        uint32_t identifier = 0x02001301;

        new_file_table.write(reinterpret_cast<char *>(&identifier), 4);
        new_file_table.write(reinterpret_cast<const char *>(&file_entry.file_header_size), 8);
        new_file_table.write(reinterpret_cast<const char *>(&file_entry.type), 12);
    }

    for (const FileData &data : file_table->file_data)
        new_file_table.write(reinterpret_cast<const char *>(&data), 12);

    new_file_table.close();
}

void DictDataManager::RepackMixedDataFile()
{
    std::vector<FileData *> sorted_data;
    for (size_t i = 0; i < file_table->global_data.size(); i++)
    {
        if (0 == file_table->global_data[i]->dict_index)
            continue;

        if (file_table->global_data[i]->has_children)
            continue;

        // skip empty indices for file entries
        if (i < file_table->file_entries.size()*2 && i % 2 != 0)
            i++;

        sorted_data.push_back(file_table->global_data[i]);
    }
    std::sort(sorted_data.begin(), sorted_data.end(), [](const FileData *a, const FileData *b) {
        return a->flags_3 < b->flags_3;
    });

    std::ofstream new_mixed_data_file(destination_directory_path+ "file3.repacked", std::ios::binary);

    uint32_t current_offset = 0;
    for (FileData *data : sorted_data)
    {
        if (data->type == 0x7100)
            continue;
        std::cout << std::hex << data->type << std::dec << ": " << (unsigned)data->dict_index << " | " << data->flags_3 << " " << data->flags_2 << "\n";
        RepackFileData(*data, new_mixed_data_file, current_offset);
        current_offset += data->flags_2;
    }

    new_mixed_data_file.close();
}

void DictDataManager::RepackFileData(FileData &file_data, std::ofstream &mixed_data_file, uint32_t offset)
{
    std::ifstream file(file_data.file_path, std::ios::binary | std::ios::ate);
    uint32_t size = file.tellg();

    file_data.flags_2 = size;
    file_data.flags_3 = offset;

    file.seekg(0, std::ios::beg);
    std::cout << mixed_data_file.tellp() << "\n";
    mixed_data_file << file.rdbuf();

    file.close();
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
} */
