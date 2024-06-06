#pragma once

class FileSystemDirectoryHandler
{
public:
    FileSystemDirectoryHandler();
    ~FileSystemDirectoryHandler() = default;

    static void CreateDirectory(const char *path);
};