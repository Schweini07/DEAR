#include "dict_data_repacker.hpp"

#include <algorithm>
#include "file_formats/dict.hpp"
#include "extracted_files/file_table.hpp"
#include "extracted_files/texture_metadata_file.hpp"
#include <zlib.h>

#include <iostream>

DictDataRepacker::DictDataRepacker(Dict *dict, FileTable *extracted_file_table, TextureMetaDataFile *extracted_texture_metadata_file, MixedDataFile *extracted_mixed_data_file, std::string directory_path)
: dict(dict), extracted_file_table(extracted_file_table), extracted_texture_metadata_file(extracted_texture_metadata_file), extracted_mixed_data_file(extracted_mixed_data_file), directory_path(directory_path)
{
}

DictDataRepacker::~DictDataRepacker()
{
}

void DictDataRepacker::Repack()
{
    RepackMixedDataFile();
    RepackTextureMetaDataFile();
    RepackFileTable();

    RepackData();

    RepackDict();   
}

void DictDataRepacker::RepackDict()
{
}

void DictDataRepacker::RepackData()
{
    std::ofstream repacked_data_file(directory_path + "repacked.data", std::ios::binary);

    for (const std::string &repacked_file_path : repacked_file_paths)
    {
        std::ifstream repacked_file(repacked_file_path, std::ios::binary | std::ios_base::ate);

        if (dict->IsCompressed())
        {
            std::streamsize file_size = repacked_file.tellg();
            repacked_data_file.seekp(0, std::ios_base::beg);

            std::vector<uint8_t> decompressed_data;
            decompressed_data.resize(file_size);
            repacked_file.read(reinterpret_cast<char *>(decompressed_data.data()), file_size);

            std::vector<uint8_t> compressed_data = CompressDataBuffer(decompressed_data);
            repacked_data_file.write(reinterpret_cast<char *>(compressed_data.data()), compressed_data.size());
        }
        else
        {
            repacked_file.seekg(0, std::ios_base::beg);
            repacked_data_file << repacked_file.rdbuf();
        }

        repacked_file.close();
    }
    
    repacked_data_file.close();
}

std::vector<uint8_t> DictDataRepacker::CompressDataBuffer(const std::vector<uint8_t> &decompressed_buffer)
{
    std::vector<uint8_t> compressed_buffer;
    uLong compressed_size = compressBound(decompressed_buffer.size());
    compressed_buffer.resize(compressed_size);

    compress(
        compressed_buffer.data(),
        reinterpret_cast<uLongf *>(&compressed_size),
        decompressed_buffer.data(),
        decompressed_buffer.size()
    );

    return compressed_buffer;
}

void DictDataRepacker::RepackFileTable()
{
    std::string file_path = directory_path + "file0.repacked";
    std::ofstream repacked_file_table(file_path, std::ios::binary);

    constexpr uint32_t IDENTIFIER = 0x02001301;
    for (const FileEntry &file_entry : extracted_file_table->file_entries)
    {
        repacked_file_table.write(reinterpret_cast<const char *>(&IDENTIFIER), 4);
        repacked_file_table.write(reinterpret_cast<const char *>(&file_entry.file_header_size), 8);
        repacked_file_table.write(reinterpret_cast<const char *>(&file_entry.type), 12);
    }

    for (const FileData &data : extracted_file_table->file_data)
        repacked_file_table.write(reinterpret_cast<const char *>(&data), 12);

    repacked_file_table.close();

    repacked_file_paths.emplace(repacked_file_paths.begin(), file_path);
}

void DictDataRepacker::RepackTextureMetaDataFile()
{
    std::string file_path = directory_path + "file2.repacked";
    std::ofstream repacked_texture_metadata_file(directory_path + "file2.repacked", std::ios::binary);

    constexpr uint32_t METADATA_IDENTIFIER = 0xE977D350;
    for (const TextureMetaData &metadata : extracted_texture_metadata_file->texture_metadata)
    {
        repacked_texture_metadata_file.write(reinterpret_cast<const char *>(&METADATA_IDENTIFIER), 4);
        repacked_texture_metadata_file.write(reinterpret_cast<const char *>(&metadata.hash), 0x34);
    }

    for (const uint8_t &byte : extracted_texture_metadata_file->other_data)
        repacked_texture_metadata_file.write(reinterpret_cast<const char *>(&byte), 1);

    repacked_texture_metadata_file.close();

    repacked_file_paths.emplace(repacked_file_paths.begin(), file_path);
}

void DictDataRepacker::RepackMixedDataFile()
{
    std::string file_path = directory_path + "file3.repacked";
    std::vector<FileData *> sorted_data;
    for (size_t i = 0; i < extracted_file_table->global_data.size(); i++)
    {
        if (0 == extracted_file_table->global_data[i]->dict_index)
            continue;

        if (extracted_file_table->global_data[i]->has_children)
            continue;

        // skip empty indices for file entries
        if (i < extracted_file_table->file_entries.size()*2 && i % 2 != 0)
            i++;

        sorted_data.push_back(extracted_file_table->global_data[i]);
    }
    std::sort(sorted_data.begin(), sorted_data.end(), [](const FileData *a, const FileData *b) {
        return a->flags_3 < b->flags_3;
    });

    std::ofstream repacked_mixed_data_file(directory_path+ "file3.repacked", std::ios::binary);

    uint32_t current_offset = 0;
    for (FileData *data : sorted_data)
    {
        if (data->type == 0x7100)
            continue;
        std::cout << std::hex << "0x" << data->type << " | " << std::dec << current_offset << "\n";
        RepackFileData(*data, repacked_mixed_data_file, current_offset);
        current_offset += data->flags_2;
    }

    repacked_mixed_data_file.close();

    repacked_file_paths.emplace(repacked_file_paths.begin(), file_path);
}

void DictDataRepacker::RepackFileData(FileData &file_data, std::ofstream &mixed_data_file, uint32_t offset)
{
    std::ifstream file(file_data.file_path, std::ios::binary | std::ios::ate);
    uint32_t size = file.tellg();

    file_data.flags_2 = size;
    file_data.flags_3 = offset;

    file.seekg(0, std::ios::beg);
    mixed_data_file << file.rdbuf();

    file.close();
}
