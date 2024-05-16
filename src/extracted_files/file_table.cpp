#include "file_table.hpp"

#include <BinaryReaderFile.h>
#include <iostream>
#include <fstream>

FileTable::FileTable(std::string file_path) : file_path(file_path)
{
}

void FileTable::Parse(std::string file_path)
{
    if ("" == file_path)
        file_path = this->file_path;

    BinaryReaderFile reader(file_path);

    // 0x020011301 is unknown magic with 14 bytes after it belonging to it
    // As it is unknown we skip it
    while (0x02001301 == reader.readUInt32())
    {
        reader.seek(20, std::ios_base::cur);
    }
    reader.seek(-4, std::ios_base::cur);

    while (reader.tell() < reader.getLength())
    {
        uint32_t identifier = reader.readUInt32();

        FileTableEntry file_table_entry;
        file_table_entry.length = reader.readUInt32();
        file_table_entry.offset = reader.readUInt32();

        switch (static_cast<Identifier>(identifier))
        {
        case Identifier::TextureHeader:
            texture_headers.push_back(file_table_entry);
            break;
        
        case Identifier::TextureData:
            texture_data.push_back(file_table_entry);
            break;

        default:
            std::cout << "Not implemented identifier: 0x" << std::hex << identifier << "\n";
            break;
        }
    }

    std::cout << std::dec;
}
