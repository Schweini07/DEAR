#include "FileSystemDirectoryHandler.hpp"

#include <filesystem>
#include <iostream>

FileSystemDirectoryHandler::FileSystemDirectoryHandler()
{
}

void FileSystemDirectoryHandler::CreateDirectory(const char *path)
{
    try
    {
        std::filesystem::path dir_path = path;
        dir_path.remove_filename();

        if (std::filesystem::exists(dir_path))
            return;

        std::filesystem::create_directories(dir_path);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
