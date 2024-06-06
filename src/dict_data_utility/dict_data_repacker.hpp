#pragma once

#include <memory>
#include <string>
#include <vector>
#include <fstream>

class Dict;
class FileTable;
class TextureMetaDataFile;
class MixedDataFile;
class FileData;

class DictDataRepacker
{
public:
    DictDataRepacker(
        FileTable *extracted_file_table,
        TextureMetaDataFile *extracted_texture_metadata_file,
        MixedDataFile *extracted_mixed_data_file,
        std::string directory_path
    );
    ~DictDataRepacker();

    void Repack();

    std::unique_ptr<Dict> dict;

private:
    void RepackDict();
    void RepackData();

    void RepackFileTable();
    void RepackTextureMetaDataFile();
    void RepackMixedDataFile();
    void RepackFileData(FileData &file_data, std::ofstream &mixed_data_file, uint32_t offset);

    void RepackFiles();

    std::vector<std::string> repacked_file_paths;

    FileTable *extracted_file_table;
    TextureMetaDataFile *extracted_texture_metadata_file;
    MixedDataFile *extracted_mixed_data_file;

    std::string directory_path;
};
