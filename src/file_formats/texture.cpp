#include "texture.hpp"

#include "bitmap.hpp"
//#include <ETC1Lib/ETC1Lib/ETC1.cpp>
//#include <lodepng/lodepng.h>
#include <fstream>
#include <iostream>

Texture::Texture(std::vector<char> texture_data, uint16_t width, uint16_t height, bool alpha)
: texture_data(texture_data), width(width), height(height), alpha(alpha)
{
    unsigned int decoded_data_size = width * height * 4;
}

void Texture::DecodeETC1()
{
    decoded_data = std::make_unique<unsigned int[]>(decoded_data_size);

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
        std::vector<char>(decoded_data.get(), decoded_data.get() + decoded_data_size),
        width,
        height,
        file_path
    );

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

    std::ofstream texture_file(file_path);
    texture_file.write(decoded_data.data(), width*height*4);
    texture_file.close();

    std::vector<unsigned char> out_data(decoded_data_buffer, decoded_data_buffer + sizeof(decoded_data_buffer)/sizeof(decoded_data_buffer[0]));
    lodepng::encode(file_path, out_data, width, height); */
}
