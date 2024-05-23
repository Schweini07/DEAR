cmake_minimum_required(VERSION 3.10)
project (DEAR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_DEBUG "-g")

include(FetchContent)

# include SFML
find_package(SFML COMPONENTS graphics QUIET)
if (NOT SFML_FOUND)
    FetchContent_Declare(
        sfml
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 69ea0cd863aed1d4092b970b676924a716ff718b
    )
    FetchContent_MakeAvailable(sfml)
endif()

# include TGUI
set(TGUI_BACKEND SFML_GRAPHICS)
set(TGUI_BUILD_FRAMEWORK FALSE)

find_package(TGUI QUIET)
if (NOT TGUI_FOUND)
    FetchContent_Declare(
        tgui
        GIT_REPOSITORY https://github.com/texus/TGUI.git
        GIT_TAG 9967a777d3a80870fc7da08ad7f29c96937143df
    )
    FetchContent_MakeAvailable(tgui)
endif()

# include zlib
find_package(ZLIB QUIET)
if (NOT ZLIB_FOUND)
    FetchContent_Declare(
        zlib
        GIT_REPOSITORY https://github.com/madler/zlib.git
        GIT_TAG 0f51fb4933fc9ce18199cb2554dacea8033e7fd3
    )
    FetchContent_MakeAvailable(zlib)

  add_library(ZLIB::ZLIB ALIAS zlib)
endif()

include_directories(src)
include_directories(lib)

add_executable(DEAR
src/main.cpp
src/dict_data_manager.cpp
src/texture_manager.cpp

src/file_formats/dict.cpp
src/file_formats/data.cpp
src/file_formats/texture.cpp
src/file_formats/bitmap.cpp

src/extracted_files/file_table.cpp
src/extracted_files/texture_metadata_file.cpp
src/extracted_files/mixed_data_file.cpp

src/gui/gui.cpp

src/utils/FileSystemDirectoryHandler.cpp
src/utils/ETC1Converter.cpp
)
target_link_libraries(DEAR Binary-Reader)
target_link_libraries(DEAR rg-etc1)
target_link_libraries(DEAR tgui)
target_link_libraries(DEAR ZLIB::ZLIB)

add_subdirectory(lib/Binary-Reader)
add_subdirectory(lib/rg-etc1)