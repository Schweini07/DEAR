#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct FileSection
{
    uint8_t id;
    uint8_t unknown;
    uint32_t offset;
    uint32_t decompressed_file_length;
    uint32_t compressed_file_length;
    uint32_t flags;

    std::string file_path;
};

class Dict
{
public:
    Dict(std::string file_path = "");
    ~Dict() = default;

    void Parse(std::string file_path = "");

    uint32_t identifier = 0xA9F32458;
    uint16_t header_flags;
    uint8_t compression_flag;
    uint32_t file_count;
    uint32_t largest_compressed_file;
    std::vector<FileSection> file_array;

private:
    std::string file_path;
};
