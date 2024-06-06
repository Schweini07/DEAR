#include "dict_data_extractor.hpp"

#include <sstream>
#include <fstream>
#include <vector>
#include <zlib.h>
#include "texture_manager.hpp"
#include "file_formats/dict.hpp"
#include "file_formats/data.hpp"
#include "extracted_files/file_table.hpp"
#include "extracted_files/texture_metadata_file.hpp"
#include "extracted_files/mixed_data_file.hpp"
#include "utils/FileSystemDirectoryHandler.hpp"

DictDataExtractor::DictDataExtractor(Dict *dict, Data *data, std::string destination_directory_path)
: dict(dict), data(data), destination_directory_path(destination_directory_path)
{
}

DictDataExtractor::~DictDataExtractor()
{
}

void DictDataExtractor::Extract()
{
    ExtractFiles();
    
    file_table = std::make_unique<FileTable>(dict->file_array[0].file_path);
    file_table->Parse();

    mixed_data = std::make_unique<MixedDataFile>(dict->file_array[3].file_path);
    mixed_data->ParseFileTable(*file_table);

    ExtractMixedData();

    texture_metadata = std::make_unique<TextureMetaDataFile>(dict->file_array[2].file_path);
    texture_metadata->Parse();
    
    texture_manager = std::make_unique<TextureManager>(*file_table, *texture_metadata);
    //ExtractTextures();
}

void DictDataExtractor::ExtractFiles()
{
    FileSystemDirectoryHandler::CreateDirectory(destination_directory_path.c_str());
    
    for (FileSection &file_section : dict->file_array)
    {
        file_section.file_path = destination_directory_path + "file" + std::to_string(file_section.id);
        ExtractDataBufferToFile(file_section);
    }
}

void DictDataExtractor::ExtractDataBufferToFile(FileSection &file_section)
{
    std::vector<uint8_t> data_buffer;
    uint32_t data_buffer_size = dict->IsCompressed() ? file_section.compressed_file_length : file_section.decompressed_file_length;
    
    data->GetDataBuffer(data_buffer, file_section.offset, data_buffer_size);

    if (dict->IsCompressed())
        data_buffer = DecompressDataBuffer(data_buffer, file_section.decompressed_file_length);

    std::ofstream extracted_file(file_section.file_path, std::ios::binary);
    extracted_file.write(reinterpret_cast<char *>(data_buffer.data()), file_section.decompressed_file_length);
    extracted_file.close();
}

std::vector<uint8_t> DictDataExtractor::DecompressDataBuffer(const std::vector<uint8_t> &compressed_buffer, uint32_t decompressed_size)
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


void DictDataExtractor::ExtractMixedData()
{
    std::string file_table_path = destination_directory_path + "file_table/";
    FileSystemDirectoryHandler::CreateDirectory(file_table_path.c_str());

    for (FileEntry &file_entry : file_table->file_entries)
    {
        std::stringstream type;
        type << std::hex << file_entry.type << std::dec << "-" << file_entry.file_header_offset;

        file_entry.file_path = file_table_path + type.str();

        std::ofstream file(file_entry.file_path, std::ios::binary);
        file.write(reinterpret_cast<const char *>(file_entry.file_header.data()), file_entry.file_header_size);
        file.write(reinterpret_cast<const char *>(file_entry.data.data()), file_entry.data.size());
        file.close();

        if (!file_entry.has_children)
            continue;
        
        ExtractDataChildren(file_entry, file_table_path);
    }
}

void DictDataExtractor::ExtractDataChildren(const FileData &file_data, const std::string &file_table_path)
{
    for (FileData *data : file_data.children)
    {
        std::stringstream type;
        type << std::hex << data->type << std::dec << "-" << data->flags_3;

        data->file_path = file_table_path + type.str();

        std::ofstream file(data->file_path, std::ios::binary);
        file.write(reinterpret_cast<const char *>(data->data.data()), data->data.size());
        file.close();

        if (!data->has_children)
            continue;
        
        ExtractDataChildren(*data, file_table_path);
    }
}

void DictDataExtractor::ExtractTextures()
{
    std::string textures_path = destination_directory_path + "textures/";
    FileSystemDirectoryHandler::CreateDirectory(textures_path.c_str());

    texture_manager->ExtractTextures(textures_path);
}
