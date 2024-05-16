#include "mixed_data_file.hpp"

#include <fstream>

MixedDataFile::MixedDataFile(std::string file_path) : file_path(file_path)
{
}

std::vector<char> MixedDataFile::GetDataBuffer(uint32_t offset, uint32_t length)
{
    std::vector<char> data_buffer(length);

    std::ifstream mixed_data_file(file_path);
    
    mixed_data_file.seekg(offset);
    mixed_data_file.read(data_buffer.data(), length);

    mixed_data_file.close();

    return data_buffer;
}
