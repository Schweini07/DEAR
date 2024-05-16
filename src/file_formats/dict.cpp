#include "dict.hpp"

#include <iostream>
#include <BinaryReaderFile.h>

Dict::Dict(std::string file_path) : file_path(file_path)
{
}

void Dict::Parse(std::string file_path)
{
    if ("" == file_path)
        file_path = this->file_path;

    BinaryReaderFile reader(file_path);

    if (identifier != reader.readUInt32())
    {
        std::cerr << "File does not contain correct identifier\n";
        return;
    }

    header_flags = reader.readUInt16();
    compression_flag = reader.readUInt8();
    reader.readUInt8(); // 1 byte padding
    file_count = reader.readUInt32();
    largest_compressed_file = reader.readUInt32();

    std::cout << unsigned(compression_flag) << " | " << file_count << " | " << largest_compressed_file << "\n"; 

    std::cout << "=========================\n";

    reader.seek(0x2b, std::ios_base::beg); // skip unknown portion of the dict

    for (size_t i = 0; i < file_count; i++)
    {
        FileSection data_file;
        data_file.id = i;
        data_file.unknown = reader.readUInt8();
        file_array.push_back(data_file);
    }

    reader.readUInt8(); // 1 byte padding
    
    for (size_t i = 0; i < file_count; i++)
    {
        file_array[i].offset =  reader.readUInt32();
        file_array[i].decompressed_file_length =  reader.readUInt32();
        file_array[i].compressed_file_length =  reader.readUInt32();
        file_array[i].flags =  reader.readUInt32();

        std::cout << file_array[i].offset << " | " << file_array[i].decompressed_file_length << " | " << file_array[i].compressed_file_length << "\n";
    }
}
