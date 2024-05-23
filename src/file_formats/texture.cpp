#include "texture.hpp"

#include <fstream>
#include <iostream>
#include "utils/ETC1Converter.hpp"
#include "bitmap.hpp"

Texture::Texture(std::vector<uint8_t> texture_data, uint16_t width, uint16_t height, bool alpha)
: texture_data(texture_data), width(width), height(height), alpha(alpha)
{
}

void Texture::DecodeETC1()
{
    std::vector<uint8_t> decoded_data;

    ETC1Converter etc1_converter;
    etc1_converter.Decode(texture_data, decoded_data, width, height, alpha);
    std::cout << "Size: " << decoded_data.size() << "\n";
    texture_data = decoded_data;
/*     ConvertETC1(
        decoded_data.get(),
        &decoded_data_size, 
        reinterpret_cast<unsigned int *>(texture_data.data()),
        width,
        height,
        alpha
    ); */
}

void Texture::Save(std::string file_path)
{
    BitMap bitmap;
    bitmap.Save(
        texture_data,
        width,
        height,
        file_path
    );

    std::ofstream test_file(file_path + ".test");
    test_file.write(reinterpret_cast<char *>(texture_data.data()), texture_data.size());
    test_file.close();

/*     unsigned int decoded_data_size = width * height * 4;
    unsigned int decoded_data_buffer[decoded_data_size];

    decoded_data.reserve(decoded_data_size);
    
    ConvertETC1(
        decoded_data_buffer,
        &decoded_data_size, 
        reinterpret_cast<unsigned int *>(texture_data.data()),
        width,
        height,
        alpha);
    std::cout << decoded_data.size() << " " << sizeof(decoded_data_buffer)/sizeof(decoded_data_buffer[0]) << "\n";

    std::ofstream texture_file(file_path, std::ios::binary);
    texture_file.write(decoded_data.data(), width*height*4);
    texture_file.close();

    std::vector<unsigned char> out_data(decoded_data_buffer, decoded_data_buffer + sizeof(decoded_data_buffer)/sizeof(decoded_data_buffer[0]));
    lodepng::encode(file_path, out_data, width, height); */
}
