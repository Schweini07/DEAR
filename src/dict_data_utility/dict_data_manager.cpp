#include "dict_data_manager.hpp"

#include <algorithm>
#include <iostream>

DictDataManager::DictDataManager(std::string dict_path, std::string data_path, std::string destination_directory_path)
: destination_directory_path(destination_directory_path)
{
    dict = std::make_unique<Dict>(dict_path);
    data = std::make_unique<Data>(data_path);

    dict->Parse();
}

void DictDataManager::Extract()
{
    extractor = std::make_unique<DictDataExtractor>(dict.get(), data.get(), destination_directory_path);
    extractor->Extract();

    texture_manager = std::make_unique<TextureManager>(*extractor->file_table.get(), *extractor->texture_metadata.get());
    texture_manager->ExtractTextures(destination_directory_path + "/textures/");
}

void DictDataManager::Repack()
{
    DictDataRepacker repacker(
        dict.get(),
        extractor->file_table.get(),
        extractor->texture_metadata.get(),
        extractor->mixed_data.get(),
        destination_directory_path
    );
    repacker.Repack();
}
