#pragma once

#include <string>

class ExtractedFile
{
public:
    ExtractedFile(std::string file_path);
    ~ExtractedFile() = default;

    std::string GetFilePath();
    void SetFilePath(std::string file_path);

protected:
    std::string file_path;
};
