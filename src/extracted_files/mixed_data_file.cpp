#include "mixed_data_file.hpp"

#include <fstream>
#include "file_table.hpp"

MixedDataFile::MixedDataFile(std::string file_path) : file_path(file_path)
{
}

void MixedDataFile::ParseFileTable(FileTable &file_table)
{
    std::ifstream mixed_data_file(file_path, std::ios::binary);

    for (FileEntry &file_entry : file_table.file_entries)
    {
        //file_entry.file_header = GetDataBuffer(file_entry.file_header_offset, file_entry.file_header_size);

        if (!file_entry.has_children)
           file_entry.data = GetDataBuffer(file_entry.flags_3, file_entry.flags_2);
    }

    for (FileData &data : file_table.file_data)
    {
        if (!data.has_children)
            data.data = GetDataBuffer(data.flags_3, data.flags_2);
    }

    mixed_data_file.close();
}

std::vector<uint8_t> MixedDataFile::GetDataBuffer(uint32_t offset, uint32_t length)
{
    std::vector<uint8_t> data_buffer(length);

    std::ifstream mixed_data_file(file_path, std::ios::binary);
    
    mixed_data_file.seekg(offset);
    mixed_data_file.read(reinterpret_cast<char *>(data_buffer.data()), length);

    mixed_data_file.close();

    return data_buffer;
}
