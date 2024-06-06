#include "file_table.hpp"

#include <BinaryReaderFile.h>
#include <iostream>
#include <fstream>

FileTable::FileTable(std::string file_path) : ExtractedFile(file_path)
{
}

void FileTable::Parse()
{
    BinaryReaderFile reader(file_path);
    uint32_t index = 0;

    while (FileEntry::identifier == reader.readUInt32())
        ParseFileEntry(reader, index);

    reader.seek(-4, std::ios_base::cur); // seek back 4 bytes as now the file entry identifier wasn't read
    
    while (reader.tell() < reader.getLength())
        ParseFileData(reader, index);
        
    for (FileData &data : file_data)
    {
        if (!data.has_children)
            continue;
        
        for (uint32_t i = 0; i < data.flags_2; i++)
            data.children.push_back(global_data[data.flags_3 + i]);
    }
    
    for (FileEntry &file_entry : file_entries)
    {
        if (!file_entry.has_children)
            continue;

        for (uint32_t i = 0; i < file_entry.flags_2; i++)
            file_entry.children.push_back(global_data[file_entry.flags_3 + i]);
    }
}

void FileTable::Write(std::string new_file_path)
{
    std::ofstream file_table_file(new_file_path, std::ios::binary);

    for (const FileEntry &file_entry : file_entries)
        WriteFileEntry(file_entry, file_table_file);
    
    for (const FileData &data : file_data)
        WriteFileData(data, file_table_file);

    file_table_file.close();
}

void FileTable::ParseFileEntry(BinaryReaderFile &reader, uint32_t &current_index)
{
    FileEntry file_entry;

    file_entry.file_header_size = reader.readUInt32();
    file_entry.file_header_offset = reader.readUInt32();
    file_entry.type = reader.readUInt16();
    file_entry.flags_1 = reader.readUInt16();
    file_entry.flags_2 = reader.readUInt32();
    file_entry.flags_3 = reader.readUInt32();

    file_entry.aligned_by_16_bytes = file_entry.flags_1 & 0x0100;
    file_entry.dict_index = (file_entry.flags_1 >> 12) & 3;
    file_entry.has_children = file_entry.flags_1 & 0x8000;

    file_entries.push_back(file_entry);

    global_data.resize(current_index + 2);
    global_data[current_index] = &file_entries.back();
    global_data[current_index+1] = &file_entries.back();

    current_index += 2;

    std::cout << "File Entry: 0x" << std::hex << file_entry.type
    << "\nSize:" << std::dec << file_entry.file_header_size
    << "\nOffset: " << file_entry.file_header_offset
    << "\nAmount of children or data size: " << file_entry.flags_2
    << "\nStart index or data offset: " << file_entry.flags_3
    << "\nHas Children:" << file_entry.has_children
    << "\nIndex: " << (unsigned)file_entry.dict_index << "\n\n";
}

void FileTable::ParseFileData(BinaryReaderFile &reader, uint32_t &current_index)
{
    FileData data;
    data.type = reader.readUInt16();
    data.flags_1 = reader.readUInt16();
    data.flags_2 = reader.readUInt32();
    data.flags_3 = reader.readUInt32();

    data.aligned_by_16_bytes = data.flags_1 & 0x0100;
    data.dict_index = (data.flags_1 >> 12) & 3;
    data.has_children = data.flags_1 & 0x8000;

    file_data.push_back(data);

    global_data.resize(current_index + 1);
    global_data[current_index] = &file_data.back();

    current_index++;

    std::cout << "File Data: 0x" << std::hex << data.type << std::dec << "\n"
    << "Data size: " << data.flags_2 << "\n"
    << "Data offset: " << data.flags_3 << "\n"
    << "Has Children: " << data.has_children << "\n"
    << "Index: " << unsigned(data.dict_index) << "\n\n";
}

void FileTable::WriteFileEntry(const FileEntry &file_entry, std::ofstream &file_table_file)
{
    uint32_t identifier = file_entry.identifier;
    file_table_file.write(reinterpret_cast<char *>(&identifier), 4);
    file_table_file.write(reinterpret_cast<const char *>(&file_entry.file_header_size), 8);
    file_table_file.write(reinterpret_cast<const char *>(&file_entry.type), 12);
}

void FileTable::WriteFileData(const FileData &data, std::ofstream &file_table_file)
{
    file_table_file.write(reinterpret_cast<const char *>(&data), 12);
}
