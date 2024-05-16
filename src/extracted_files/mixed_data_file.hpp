#pragma once

#include <string>
#include <cstdint>
#include <vector>

class MixedDataFile
{
public:
    MixedDataFile(std::string file_path = "");
    ~MixedDataFile() = default;

    std::vector<char> GetDataBuffer(uint32_t offset, uint32_t length);

private:
    std::string file_path;
};
