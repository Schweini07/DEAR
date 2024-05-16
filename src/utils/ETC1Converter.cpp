#include "ETC1Converter.hpp"

#include <cstring>

ETC1Converter::ETC1Converter()
{
}

// using https://github.com/gdkchan/Ohana3DS-Rebirth/blob/master/Ohana3DS%20Rebirth/Ohana/TextureCodec.cs
void ETC1Converter::Decode(std::vector<char> compressed_data, std::vector<char> &decompressed_data, uint16_t width, uint16_t height, bool alpha)
{
    uint32_t offset;

    for (uint16_t y = 0; y < height/4; y++)
    {
        for (uint16_t x = 0; x < width/4; x++)
        {
            uint64_t *color_block, *alpha_block;

            if (alpha)
            {
                for (int i = 0; i < 8; i++)
                {
                    color_block[7-i] = compressed_data[offset + 8 + i];
                    alpha_block[i] = compressed_data[offset + i];
                }
                offset += 16;
            }
            else
            {
                for (int i = 0; i < 8; i++)
                {
                    color_block[7-i] = compressed_data[offset + i];
                    alpha_block[i] = 0xFF;
                }
                offset += 8;
            }

            DecodeBlock(color_block);
        }
    }
}

void ETC1Converter::DecodeBlock(uint64_t *block)
{
    /*
    0x0 0x0 0x0 0x0 <-- Top block
    0x0 0x0 0x0 0x0 <-- Bottom block
    */

   uint32_t *top_block;
   std::memcpy(top_block, block, sizeof(uint64_t)/2);
   
   uint32_t *bottom_block;
   std::memcpy(bottom_block, block, sizeof(uint64_t)/2);


   bool flip = (*top_block & 0x10000000) > 0;
   bool difference = (*top_block & 0x20000000) > 0;

   uint32_t r1, g1, b1;
   uint32_t r2, g2, b2;

   if (difference)
   {
    r1 = *top_block & 0xF8;
    g1 = (*top_block & 0xF800) >> 8;
    g1 = (*top_block & 0xF80000) >> 16;

    
   }
}
