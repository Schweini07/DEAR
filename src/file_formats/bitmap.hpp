#pragma once

#include <string>
#include <cstdint>
#include <vector>

// using https://dev.to/muiz6/c-how-to-write-a-bitmap-image-from-scratch-1k6m

struct Header
    {
        const uint16_t identifier = 0x4D42;
        uint32_t length;
        uint32_t reserved_bytes = 0;
        uint32_t data_offset = 54;

        void SaveToFile(std::ofstream &file);
    };

    struct InfoHeader
    {
        uint32_t header_length = 40;
        uint32_t width;
        uint32_t height;
        uint16_t number_of_color_planes = 1;
        uint16_t color_depth = 32;
        uint32_t compression_format = 0;
        uint32_t raw_data_size;
        uint32_t horizontal_resolution = 0;
        uint32_t vertical_resolution = 0;
        uint32_t color_table_entries = 0;
        uint32_t important_colors = 0;

        void SaveToFile(std::ofstream &file);
    };

    struct Pixel
    {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
        uint8_t alpha;

        void SaveToFile(std::ofstream &file);
    };

class BitMap
{
public:
    BitMap();
    ~BitMap() = default;

    void Save(const std::vector<uint8_t> &data, uint32_t width, uint32_t height, std::string file_path);
};
