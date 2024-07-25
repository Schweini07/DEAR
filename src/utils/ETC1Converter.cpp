#include "ETC1Converter.hpp"

#include <cstring>
#include <iostream>
#include <rg-etc1/rg_etc1.h>
#include <fstream>

ETC1Converter::ETC1Converter()
{
}

void ETC1Converter::Decode(std::vector<uint8_t> compressed_data, std::vector<uint8_t> &decompressed_data, uint16_t width, uint16_t height, bool alpha)
{
    decompressed_data.resize(width * height * 4);

    uint32_t offset = 0;

    for (uint16_t y = 0; y < height/4; y++)
    {
        for (uint16_t x = 0; x < width/4; x++)
        {
            if (false)
            {
                uint8_t decoded_alpha_block[64];
                std::vector<uint8_t> alpha_block(compressed_data.begin()+offset, compressed_data.begin()+offset+8);

                bool success = rg_etc1::unpack_etc1_block(
                    alpha_block.data(),
                    reinterpret_cast<uint32_t *>(decoded_alpha_block),
                    true
                );

                if (!success)
                    std::cerr << "Wasn't able to unpack ETC1 alpha block at offset " << offset << "!\n";
                
                decompressed_data.insert(decompressed_data.end(), decoded_alpha_block, decoded_alpha_block + 64);

                offset += 8;
            }

            uint8_t decoded_block[64];
            std::vector<uint8_t> encoded_block;
            for (int i = 7; i >= 0; i--)
                encoded_block.push_back(compressed_data[i + offset]);

            bool success = rg_etc1::unpack_etc1_block(
                encoded_block.data(),
                reinterpret_cast<uint32_t *>(decoded_block),
                false
            );

            if (!success)
                std::cerr << "Wasn't able to unpack ETC1 block at offset " << offset << "!\n";
            
            for (uint32_t decoded_block_y = 0; decoded_block_y < 4; decoded_block_y++)
            {
                for (uint32_t decoded_block_x = 0; decoded_block_x < 4; decoded_block_x++)
                {
                    for (uint8_t byte = 0; byte < 4; byte++)
                    {
                        uint32_t source_position = 4 * (decoded_block_x + decoded_block_y*4) + byte;
                        uint32_t destination_position = ((y*4 + decoded_block_y) * width + x*4 + decoded_block_x)*4 + byte;

                        decompressed_data[destination_position] = decoded_block[source_position];
                    }
                }
            }
            offset += 8; 
        }
    }
}
