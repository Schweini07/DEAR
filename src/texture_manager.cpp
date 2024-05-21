#include "texture_manager.hpp"

#include "extracted_files/file_table.hpp"
#include "extracted_files/texture_metadata_file.hpp"
#include "extracted_files/file_constants.hpp"
#include "file_formats/texture.hpp"

TextureManager::TextureManager(FileTable &file_table, TextureMetaDataFile &texture_metadata_file)
{
    this->file_table = &file_table;
    texture_metadata_files = &texture_metadata_file.texture_metadata;
}

void TextureManager::ExtractTextures(std::string file_path)
{
    int index = 0;
    for (const FileEntry &file_entry : file_table->file_entries)
    {
        if (static_cast<uint16_t>(FileConstants::TEXTURE_FILE) != file_entry.type)
            return;
        
        TextureMetaData meta_data = texture_metadata_files->at(index);
        
        Texture texture(file_entry.children[1]->data, meta_data.width, meta_data.height, false);
        texture.Save(file_path + "texture" + std::to_string(index));

        index++;
    }
}
