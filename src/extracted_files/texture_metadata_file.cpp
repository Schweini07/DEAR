#include "texture_metadata_file.hpp"

#include <BinaryReaderFile.h>
#include <iostream>

TextureMetaDataFile::TextureMetaDataFile(std::string file_path) : file_path(file_path)
{

}

void TextureMetaDataFile::Parse(std::string file_path)
{
    if ("" == file_path)
        file_path = this->file_path;
    
    BinaryReaderFile reader(file_path);
    
    while (reader.readUInt32() == TextureMetaData::identifier)
    {
        ParseMetaData(reader);
    } 
}

void TextureMetaDataFile::ParseMetaData(BinaryReaderFile &reader)
{
    TextureMetaData meta_data;

    meta_data.hash = reader.readUInt32();
    meta_data.length = reader.readUInt32();

    if (meta_data.hash != reader.readUInt32())
    {
        std::cerr << "Expected hash here, but was not found! Something is different in this texture metadata file.\n";
        return;
    }

    reader.readUInt32(); // 4 byte padding
    reader.readUInt32(); // unknown

    meta_data.width = reader.readUInt16();
    meta_data.height = reader.readUInt16();

    reader.readUInt16(); // 2 byte padding
    reader.readUInt8(); // unknown

    meta_data.mipmap_level = reader.readUInt8();

    reader.seek(20, std::ios_base::cur);

    meta_data.compression_format = reader.readUInt32();

    texture_metadata.push_back(meta_data);
}
