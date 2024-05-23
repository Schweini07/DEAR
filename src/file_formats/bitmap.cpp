#include "bitmap.hpp"

#include <fstream>

void Header::SaveToFile(std::ofstream &file)
{
    file.write((char *)&this->identifier, sizeof(uint16_t));
    file.write((char *)&this->length, sizeof(uint32_t));
    file.write((char *)&this->reserved_bytes, sizeof(uint32_t));
    file.write((char *)&this->data_offset, sizeof(uint32_t));
}

void InfoHeader::SaveToFile(std::ofstream &file)
{
    file.write((char *)&this->header_length, sizeof(uint32_t));
    file.write((char *)&this->width, sizeof(uint32_t));
    file.write((char *)&this->height, sizeof(uint32_t));
    file.write((char *)&this->number_of_color_planes, sizeof(uint16_t));
    file.write((char *)&this->color_depth, sizeof(uint16_t));
    file.write((char *)&this->compression_format, sizeof(uint32_t));
    file.write((char *)&this->raw_data_size, sizeof(uint32_t));
    file.write((char *)&this->horizontal_resolution, sizeof(uint32_t));
    file.write((char *)&this->vertical_resolution, sizeof(uint32_t));
    file.write((char *)&this->color_table_entries, sizeof(uint32_t));
    file.write((char *)&this->important_colors, sizeof(uint32_t));
}

void Pixel::SaveToFile(std::ofstream &file)
{
    file.write((char *)&this->blue, sizeof(uint8_t));
    file.write((char *)&this->green, sizeof(uint8_t));
    file.write((char *)&this->red, sizeof(uint8_t));
    file.write((char *)&this->alpha, sizeof(uint8_t));
}

BitMap::BitMap()
{
}

void BitMap::Save(const std::vector<uint8_t> &data, uint32_t width, uint32_t height, std::string file_path)
{
    std::ofstream bitmap_file(file_path + ".bmp", std::ios::binary);

    Header header;
    header.length = width * height;
    header.SaveToFile(bitmap_file);

    InfoHeader info_header;
    info_header.width = width;
    info_header.height = height;
    info_header.raw_data_size = data.size();
    info_header.SaveToFile(bitmap_file);

    for (size_t i = 3; i < width*height*4; i+=4)
    {
        Pixel pixel;
        pixel.alpha = data.data()[i];
        pixel.blue = data.data()[i-1];
        pixel.green = data.data()[i-2];
        pixel.red = data.data()[i-3];

        pixel.SaveToFile(bitmap_file);
    }

    bitmap_file.close();
}
