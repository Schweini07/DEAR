#include "dict.hpp"

#include <iostream>
#include <BinaryReaderFile.h>

Dict::Dict(std::string file_path) : file_path(file_path)
{
}

void Dict::Parse()
{
    BinaryReaderFile reader(file_path);

    if (header.identifier != reader.readUInt32())
    {
        std::cerr << "File does not contain correct identifier\n";
        return;
    }

    header.header_flags = reader.readUInt16();
    header.compression_flag = reader.readUInt8();
    header.padding_1 = reader.readUInt8(); // 1 byte padding
    header.file_count = reader.readUInt32();
    header.largest_compressed_file_size = reader.readUInt32();
    header.file_table_count = reader.readUInt8();
    header.padding_2 = reader.readUInt8();
    header.file_table_reference_count = reader.readUInt8();
    header.file_extension_count = reader.readUInt8();

    std::cout << unsigned(header.compression_flag) << " | " << header.file_count << " | " << header.largest_compressed_file_size << "\n"; 

    std::cout << "=========================\n";

    for (uint8_t i = 0; i < header.file_table_reference_count; i++)
    {
        FileTableReference file_table_reference;
        file_table_reference.hash = reader.readUInt32();
        
        for (uint8_t j = 0; j < 8; j++)
            file_table_reference.file_indices.push_back(reader.readUInt8());
    }

    for (uint8_t i = 0; i < header.file_count; i++)
    {
        FileSection data_file;
        data_file.id = i;
        data_file.unknown = reader.readUInt8();
        file_array.push_back(data_file);
    }
    
    for (uint8_t i = 0; i < header.file_count; i++)
    {
        file_array[i].offset =  reader.readUInt32();
        file_array[i].decompressed_file_length =  reader.readUInt32();
        file_array[i].compressed_file_length =  reader.readUInt32();
        file_array[i].flags =  reader.readUInt32();

        std::cout << file_array[i].offset << " | " << file_array[i].decompressed_file_length << " | " << file_array[i].compressed_file_length << "\n";
    }
}

void Dict::Write()
{
    std::ofstream dict_file(file_path);

}
