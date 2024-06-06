#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <cstdint>

class Dict;
class FileTable;
class TextureMetaDataFile;
class MixedDataFile;
class FileData;

class DictDataRepacker
{
public:
    DictDataRepacker(
        Dict *dict,
        FileTable *extracted_file_table,
        TextureMetaDataFile *extracted_texture_metadata_file,
        MixedDataFile *extracted_mixed_data_file,
        std::string directory_path
    );
    ~DictDataRepacker();

    void Repack();

private:
    void RepackDict();
    void RepackData();
    std::vector<uint8_t> CompressDataBuffer(const std::vector<uint8_t> &decompressed_buffer);

    void RepackFileTable();
    void RepackTextureMetaDataFile();
    void RepackMixedDataFile();
    void RepackFileData(FileData &file_data, std::ofstream &mixed_data_file, uint32_t offset);

    void RepackFiles();

    std::vector<std::string> repacked_file_paths;

    Dict *dict;
    FileTable *extracted_file_table;
    TextureMetaDataFile *extracted_texture_metadata_file;
    MixedDataFile *extracted_mixed_data_file;

    std::string directory_path;
};
