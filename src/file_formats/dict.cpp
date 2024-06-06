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
        
        header.file_table_references.push_back(file_table_reference);
    }

    for (uint8_t i = 0; i < header.file_count; i++)
    {
        FileSection data_file;

        data_file.id = i;
        data_file.unknown_1 = reader.readUInt8();

        file_array.push_back(data_file);
    }
    
    for (uint8_t i = 0; i < header.file_count; i++)
    {
        file_array[i].offset =  reader.readUInt32();
        file_array[i].decompressed_file_length =  reader.readUInt32();
        file_array[i].compressed_file_length =  reader.readUInt32();
        file_array[i].type =  reader.readUInt8();
        file_array[i].padding =  reader.readUInt8();
        file_array[i].file_extension =  reader.readUInt8();
        file_array[i].unknown_2 =  reader.readUInt8();

        std::cout << file_array[i].offset << " | " << file_array[i].decompressed_file_length << " | " << file_array[i].compressed_file_length << "\n";
    }
}

void Dict::Write(std::string new_dict_path)
{
    std::ofstream dict_file(new_dict_path, std::ios::binary);

    dict_file.write(reinterpret_cast<char *>(&header), 0x14);

    for (const FileTableReference &file_table_reference : header.file_table_references)
    {
        dict_file.write(reinterpret_cast<const char *>(&(file_table_reference.hash)), 0x4);

        for (const uint8_t &file_index : file_table_reference.file_indices)
            dict_file.write(reinterpret_cast<const char *>(&file_index), 0x1);
    }

    for (const FileSection &file_section : file_array)
        dict_file.write(reinterpret_cast<const char *>(&file_section), 0x1);
    
    for (const FileSection &file_section : file_array)
        dict_file.write(reinterpret_cast<const char *>(&file_section.offset), 0x10);

    dict_file.write(".data", 5);
    dict_file.put(0);

    dict_file.write(".debug", 6);
    dict_file.put(0);
    
    dict_file.close();
}

bool Dict::IsCompressed()
{
    return header.compression_flag;
}
