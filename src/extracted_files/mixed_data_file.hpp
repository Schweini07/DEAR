#pragma once

#include "extracted_file.hpp"

#include <string>
#include <cstdint>
#include <vector>

class FileTable;

class MixedDataFile : public ExtractedFile
{
public:
    MixedDataFile(std::string file_path = "");
    ~MixedDataFile() = default;

    void ParseFileTable(FileTable &file_table);
    std::vector<uint8_t> GetDataBuffer(uint32_t offset, uint32_t length);
};
