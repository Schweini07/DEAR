#include "gui.hpp"
#include "file_formats/texture.hpp"
#include "extracted_files/file_table.hpp"
#include "extracted_files/texture_metadata_file.hpp"
#include "extracted_files/mixed_data_file.hpp"

GUI::GUI()
{
    dark_theme.load("resources/themes/Dark.txt");
}

void GUI::StartApplication()
{
    window = std::make_unique<sf::RenderWindow>(window_size, window_title);
    LoadWindowIcon();

    gui = std::make_unique<tgui::Gui>(*window);
    gui->onViewChange([this] {
        const float CURRENT_SCREEN_HEIGHT = gui->getView().getRect().height;

        gui->setTextSize(static_cast<unsigned int>(13.f * (CURRENT_SCREEN_HEIGHT / window_size.height)));
        //tgui::getBackend()->setFontScale(CURRENT_SCREEN_HEIGHT / static_cast<float>(window_size.height));
    });

    tgui::Theme::setDefault("resources/themes/Dark.txt");

    LoadMainForm(); 

    while (window->isOpen())
    {
        sf::Event event;

        while (window->pollEvent(event))
        {
            gui->handleEvent(event);

            if (event.type == sf::Event::Closed)
                window->close();
        }

        window->clear();

        gui->draw();

        window->display();
    }
}

void GUI::LoadWindowIcon()
{
    sf::Image image = sf::Image();
    if (!image.loadFromFile(window_icon_path))
    {
        std::cerr << "Unable to load window icon.\n";
    }

    window->setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
}

void GUI::LoadMainForm()
{
    gui->loadWidgetsFromFile("resources/forms/main_form.txt");
    load_dict_button = gui->get<tgui::Button>("LoadDictButton");
    load_dict_button->onPress([this] {
        tgui::FileDialog::Ptr file_dialog = tgui::FileDialog::create("Select .dict file", "Select", false);
        file_dialog->setFileTypeFilters({
            {"NLG dictionary (*.dict)", {"*.dict"}}
        });
        file_dialog->onFileSelect([this, file_dialog] {
           load_dict_button->setText(file_dialog->getSelectedPaths()[0].getFilename());
           dict_file_path = file_dialog->getSelectedPaths()[0].asString();

           HandleStartExtractionButtonState();
        });

        gui->add(file_dialog);
    });

    load_data_button = gui->get<tgui::Button>("LoadDataButton");
    load_data_button->onPress([this] {
        tgui::FileDialog::Ptr file_dialog = tgui::FileDialog::create("Select .data file", "Select", false);
        file_dialog->setFileTypeFilters({
            {"NLG data file (*.data)", {"*.data"}}
        });
        file_dialog->onFileSelect([this, file_dialog] {
           load_data_button->setText(file_dialog->getSelectedPaths()[0].getFilename());
           data_file_path = file_dialog->getSelectedPaths()[0].asString();
           
           HandleStartExtractionButtonState();
        });
        
        gui->add(file_dialog);
    });

    directory_path_edit_box = gui->get<tgui::EditBox>("DirectoryPathEditBox");
    directory_path_edit_box->onTextChange([this] {
        destination_directory_path = directory_path_edit_box->getText();

        HandleStartExtractionButtonState();
    });

    directory_path_button = gui->get<tgui::Button>("DirectoryPathButton");
    directory_path_button->onPress([this] {
        tgui::FileDialog::Ptr file_dialog = tgui::FileDialog::create("Choose directory", "Select", true);
        file_dialog->setSelectingDirectory(true);

        file_dialog->onFileSelect([this, file_dialog] {
            directory_path_edit_box->setText(file_dialog->getSelectedPaths()[0].asString());
        });

        gui->add(file_dialog);
    });

    start_extraction_button = gui->get<tgui::Button>("StartExtractionButton");
    HandleStartExtractionButtonState();
    start_extraction_button->onPress([this] {        
        ExtractFiles();
        repack_files_button->setEnabled(true);
    });


    extracted_files_panel_list_box = gui->get<tgui::PanelListBox>("ExtractedFilesPanelListBox");
    extracted_files_panel_list_box->setRenderer(dark_theme.getRenderer("Panel"));

    repack_files_button = gui->get<tgui::Button>("RepackFilesButton");
    if (!start_extraction_button->isEnabled())
    {
        repack_files_button->setEnabled(false);
    }
    repack_files_button->onPress([this] {
        dict_data_manager->RepackFiles();
    });
}

void GUI::InitExtractedFilesPanelListBox(const std::vector<FileSection> &extracted_files)
{
    for (const FileSection &data_file : extracted_files)
    {
        // Conversion is needed, because else no number is displayed for some reason
        tgui::String id(data_file.id);
        tgui::String file_name = "File" + id;

        tgui::Panel::Ptr item_panel = extracted_files_panel_list_box->addItem(id);

        tgui::Label::Ptr file_name_label = tgui::Label::create(file_name);
        item_panel->add(file_name_label);
    }
}

void GUI::HandleStartExtractionButtonState()
{
    if (dict_file_path == "" || data_file_path == "" || destination_directory_path == "")
    {
        start_extraction_button->setEnabled(false);
        return;
    }
    
    start_extraction_button->setEnabled(true);
}

void GUI::ExtractFiles()
{
    dict_data_manager = std::make_unique<DictDataManager>(dict_file_path.toStdString(), data_file_path.toStdString(), destination_directory_path.toStdString());
    dict_data_manager->ExtractFiles();

/*     FileTable file_table(dict->file_array[0].file_path);
    file_table.Parse();

    TextureMetaDataFile text_metadata_file(dict->file_array[2].file_path);
    text_metadata_file.Parse();

    for (size_t i = 0; i < file_table.texture_data.size(); i++)
    {
        FileTableEntry texture_entry = file_table.texture_data[i];
        TextureMetaData texture_metadata = text_metadata_file.texture_metadata[i];

        ExtractTexture(texture_entry, texture_metadata, dict->file_array[3].file_path);
    } */

    //InitExtractedFilesPanelListBox(dict->file_array);
}

void GUI::ExtractTexture(FileTableEntry &texture_entry, TextureMetaData texture_metadata, std::string mixed_data_file_path)
{
/*     MixedDataFile mixed_data_file(mixed_data_file_path);

    Texture texture(
        mixed_data_file.GetDataBuffer(texture_entry.offset, texture_entry.length),
        texture_metadata.width,
        texture_metadata.height,
        false
    );

    texture.DecodeETC1();
    texture.Save(mixed_data_file_path + std::to_string(texture_metadata.hash)); */
}
