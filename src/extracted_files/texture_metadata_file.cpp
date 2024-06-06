#include "texture_metadata_file.hpp"

#include <BinaryReaderFile.h>
#include <iostream>

TextureMetaDataFile::TextureMetaDataFile(std::string file_path) : ExtractedFile(file_path)
{

}

void TextureMetaDataFile::Parse()
{   
    BinaryReaderFile reader(file_path);
    
    while (reader.readUInt32() == TextureMetaData::identifier)
    {
        ParseMetaData(reader);
    }

    reader.seek(-4, std::ios_base::cur);

    while (reader.tell() != reader.getLength())
        other_data.push_back(reader.readUInt8());
}

void TextureMetaDataFile::ParseMetaData(BinaryReaderFile &reader)
{
    TextureMetaData meta_data;

    meta_data.hash = reader.readUInt32();
    meta_data.length = reader.readUInt32();
    meta_data.hash2 = reader.readUInt32();

    if (meta_data.hash != meta_data.hash2)
    {
        std::cerr << "Hashes do not match each other! Something is wrong with this texture metadata file.\n";
        return;
    }

    meta_data.padding = reader.readUInt32();
    meta_data.unknown = reader.readUInt32();

    meta_data.width = reader.readUInt16();
    meta_data.height = reader.readUInt16();

    meta_data.padding2 = reader.readUInt16();
    meta_data.unknown2 = reader.readUInt8();

    meta_data.mipmap_level = reader.readUInt8();

    meta_data.unknown3 = reader.readUInt32();
    meta_data.unknown4 = reader.readUInt32();
    meta_data.unknown5 = reader.readUInt32();
    meta_data.unknown6 = reader.readUInt32();
    meta_data.unknown7 = reader.readUInt32();

    meta_data.compression_format = reader.readUInt32();

    texture_metadata.push_back(meta_data);
}
