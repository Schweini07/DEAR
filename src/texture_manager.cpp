#include "texture_manager.hpp"

#include <cmath>
#include <iostream>
#include "extracted_files/file_table.hpp"
#include "extracted_files/texture_metadata_file.hpp"
#include "extracted_files/file_constants.hpp"
#include "utils/FileSystemDirectoryHandler.hpp"

TextureManager::TextureManager(FileTable &file_table, TextureMetaDataFile &texture_metadata_file)
{
    this->file_table = &file_table;
    texture_metadata_files = &texture_metadata_file.texture_metadata;
}

void TextureManager::ExtractTextures(std::string file_path)
{
    FileSystemDirectoryHandler::CreateDirectory(file_path.c_str());

    int index = 0;
    for (const FileEntry &file_entry : file_table->file_entries)
    {
        if (static_cast<uint16_t>(FileConstants::TEXTURE_FILE) != file_entry.type)
            return;
        
        TextureMetaData meta_data = texture_metadata_files->at(index);
        bool has_alpha = (13 == meta_data.compression_format) ? true : false;

        Texture texture(file_entry.children[1]->data, meta_data.width, meta_data.height, has_alpha);
        texture.DecodeETC1();
        texture.Save(file_path + "texture" + std::to_string(index));

        index++;
    }
}
