#pragma once

#include <memory>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "dict_data_manager.hpp"

class FileTableEntry;
class TextureMetaData;
class FileSection;

class GUI
{
public:
    GUI();
    ~GUI() = default;

    void StartApplication();

private:
    void LoadMainForm();
    void InitExtractedFilesPanelListBox(const std::vector<FileSection> &extracted_files);
    void HandleStartExtractionButtonState();

    void ExtractFiles();
    void ExtractTexture(FileTableEntry &texture_entry, TextureMetaData texture_metadata, std::string mixed_data_file_path);

private:
    std::unique_ptr<tgui::Gui> gui;
    tgui::Theme dark_theme;

    tgui::Button::Ptr load_dict_button;
    tgui::Button::Ptr load_data_button;
    tgui::EditBox::Ptr directory_path_edit_box;
    tgui::Button::Ptr directory_path_button;
    tgui::Button::Ptr start_extraction_button;

    tgui::PanelListBox::Ptr extracted_files_panel_list_box;
    tgui::Button::Ptr repack_files_button;

    tgui::String dict_file_path = "/home/laurenz/Dokumente/Programmieren/3DS-Modding/DarkMoon/Tools/data_extractor/resources/data_and_dict_files/mansione_kingboo/kingboo.dict";
    tgui::String data_file_path = "/home/laurenz/Dokumente/Programmieren/3DS-Modding/DarkMoon/Tools/data_extractor/resources/data_and_dict_files/mansione_kingboo/kingboo.data";
    tgui::String destination_directory_path = "/home/laurenz/Dokumente/Programmieren/3DS-Modding/DarkMoon/Tools/data_extractor/data_and_dict_files/kingboo/";


    std::unique_ptr<DictDataManager> dict_data_manager;
};

