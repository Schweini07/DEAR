#include "data.hpp"

#include "dict.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <zlib.h>

Data::Data(std::string file_path) : file_path(file_path)
{
    std::ifstream data_file(file_path, std::ifstream::ate);
    file_size = data_file.tellg();
    data_file.close();

    std::cout << file_size << "\n";
}

void Data::GetDataBuffer(std::vector<uint8_t> &data_buffer, uint32_t offset, uint32_t buffer_size)
{
    data_buffer.resize(buffer_size);

    std::ifstream data_file(file_path);

    data_file.seekg(offset);
    data_file.read(reinterpret_cast<char *>(data_buffer.data()), buffer_size);

    data_file.close();
}

uint32_t Data::GetFileSize()
{
    return file_size;
}
