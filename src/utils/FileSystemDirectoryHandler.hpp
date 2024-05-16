#pragma once

class FileSystemDirectoryHandler
{
public:
    FileSystemDirectoryHandler();
    ~FileSystemDirectoryHandler() = default;

    void CreateDirectory(const char *path);
};