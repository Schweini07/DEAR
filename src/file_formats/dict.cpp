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

    std::cout << "Compressed: " << unsigned(header.compression_flag)
    << "\nFile Count: " << header.file_count
    << "\nLargest Compressed File Size: " << header.largest_compressed_file_size
    << "\n"; 

    std::cout << "=========================\n";

    for (uint8_t i = 0; i < header.file_table_reference_count; i++)
    {
        FileTableReference file_table_reference;
        file_table_reference.hash = reader.readUInt32();
        
        for (uint8_t j = 0; j < 8; j++)
            file_table_reference.file_indices.push_back(reader.readUInt8());
        
        file_table_references.push_back(file_table_reference);
    }

    for (uint8_t i = 0; i < header.file_table_reference_count; i++)
    {
        FileTableInfo table_info;

        table_info.file_count = reader.readUInt16();
        table_info.block_index = reader.readUInt16();

        file_table_info.push_back(table_info);
    }
    
    for (uint8_t i = 0; i < header.file_count; i++)
    {
        FileSection file_section;
        file_section.id = i;
        file_section.offset =  reader.readUInt32();
        file_section.decompressed_file_length =  reader.readUInt32();
        file_section.compressed_file_length = reader.readUInt32();    
        file_section.type =  reader.readUInt8();
        file_section.padding =  reader.readUInt8();
        file_section.file_extension =  reader.readUInt8();
        file_section.unknown_2 =  reader.readUInt8();

        file_array.push_back(file_section);

        std::cout << "Offset: " << file_array[i].offset 
        << "\nDecompressed File Length: " << file_array[i].decompressed_file_length
        << "\nCompressed File Length: " << file_array[i].compressed_file_length
        << "\nDebug: " << unsigned(file_array[i].file_extension)
        << "\n\n";
    }
}

void Dict::Write(std::string new_dict_path)
{
    std::ofstream dict_file(new_dict_path, std::ios::binary);

    dict_file.write(reinterpret_cast<char *>(&header), 0x14);

    for (const FileTableReference &file_table_reference : file_table_references)
    {
        dict_file.write(reinterpret_cast<const char *>(&(file_table_reference.hash)), 0x4);

        for (const uint8_t &file_index : file_table_reference.file_indices)
            dict_file.write(reinterpret_cast<const char *>(&file_index), 0x1);
    }

    for (const FileTableInfo &info : file_table_info)
        dict_file.write(reinterpret_cast<const char *>(&info), 0x4);
    
    for (const FileSection &file_section : file_array)
        dict_file.write(reinterpret_cast<const char *>(&file_section), 0x10);

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
