#pragma once

#include <cstdint>

enum class FileConstants : uint16_t
{
    TEXTURE_FILE = 0xB500,
    TEXTURE_HEADER = 0xB501,
    TEXTURE_DATA = 0xB502
};