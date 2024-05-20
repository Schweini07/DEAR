#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <deque>

class Data;
class BinaryReaderFile;

class FileData
{
public:
    uint16_t type;
    uint16_t flags_1;
    uint32_t flags_2; // children count or data size
    uint32_t flags_3; // children start index or data offset

    bool aligned_by_16_bytes;
    uint8_t dict_index;
    bool has_children;

    std::vector<FileData *> children;
    std::vector<uint8_t> data;
};

class FileEntry : public FileData
{
public:
    static const uint32_t identifier = 0x02001301;
    uint32_t file_header_size;
    uint32_t file_header_offset;

    std::vector<uint8_t> file_header;
};

class FileTable
{
public:
    FileTable(std::string file_path = "");
    ~FileTable() = default;

    void Parse();
    void Write(std::string new_file_path);

    std::vector<FileData *> global_data;

    // std::deque is just a vector with a different memory allcoation strategy, which we need because of the way we use this vectors
    std::deque<FileEntry> file_entries;
    std::deque<FileData> file_data;

private:
    void ParseFileEntry(BinaryReaderFile &reader, uint32_t &current_index);
    void ParseFileData(BinaryReaderFile &reader, uint32_t &current_index);

    void WriteFileEntry(const FileEntry &file_entry, std::ofstream &file_table_file);
    void WriteFileData(const FileData &data, std::ofstream &file_table_file);

    std::string file_path;
};
