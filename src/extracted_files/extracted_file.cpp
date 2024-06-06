#include "extracted_file.hpp"

ExtractedFile::ExtractedFile(std::string file_path) : file_path(file_path)
{
}

std::string ExtractedFile::GetFilePath()
{
    return file_path;
}

void ExtractedFile::SetFilePath(std::string file_path)
{
    this->file_path = file_path;
}
