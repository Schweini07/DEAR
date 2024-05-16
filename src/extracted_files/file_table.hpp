#pragma once

#include <string>
#include <cstdint>
#include <vector>

class Data;

class FileTableEntry
{
public:
    uint32_t length;
    uint32_t offset;
};

enum class Identifier : uint32_t
{
    TextureHeader = 0x0201B501,
    TextureData = 0x1701B502,
};

class FileTable
{
public:
    FileTable(std::string file_path = "");
    ~FileTable() = default;

    void Parse(std::string file_path = "");

    std::vector<FileTableEntry> texture_headers;
    std::vector<FileTableEntry> texture_data;

private:
    std::string file_path;
};
