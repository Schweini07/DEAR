#pragma once

#include <string>
#include <memory>
#include <vector>
#include "file_formats/dict.hpp"
#include "file_formats/data.hpp"
#include "dict_data_extractor.hpp"
#include "dict_data_repacker.hpp"

class Dict;
class Data;

class DictDataManager
{
public:
    DictDataManager(std::string dict_path, std::string data_path, std::string destination_directory_path);
    ~DictDataManager() = default;

    void Extract();
    void Repack();

private:
    std::unique_ptr<Dict> dict;
    std::unique_ptr<Data> data;
    
    std::unique_ptr<DictDataExtractor> extractor;

    std::string destination_directory_path;
};
