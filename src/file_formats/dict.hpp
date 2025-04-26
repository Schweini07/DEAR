#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct FileSection
{
    uint32_t offset;
    uint32_t decompressed_file_length;
    uint32_t compressed_file_length;
    uint8_t type;
    uint8_t padding;
    uint8_t file_extension;
    uint8_t unknown_2;

    uint8_t id;
    std::string file_path;
};

struct FileTableInfo
{
    uint16_t file_count;
    uint16_t block_index;
};

struct FileTableReference
{
    uint32_t hash;
    std::vector<uint8_t> file_indices;
};

struct DictHeader
{
    uint32_t identifier = 0xA9F32458;
    uint16_t header_flags;
    uint8_t compression_flag;
    uint8_t padding_1;
    uint32_t file_count;
    uint32_t largest_compressed_file_size;
    uint8_t file_table_count;
    uint8_t padding_2;
    uint8_t file_table_reference_count;
    uint8_t file_extension_count;
};

class Dict
{
public:
    Dict(std::string file_path);
    ~Dict() = default;

    void Parse();
    void Write(std::string new_dict_path);

    DictHeader header;
    std::vector<FileTableReference> file_table_references;
    std::vector<FileTableInfo> file_table_info;
    std::vector<FileSection> file_array;

    bool IsCompressed();

private:
    std::string file_path;
};
