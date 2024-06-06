#include "file_selector.hpp"

FileSelector::FileSelector(const char *typeName, bool initRenderer) :
    tgui::HorizontalLayout(typeName, initRenderer)
{
    file_icon = tgui::Picture::create("resources/images/gui/file_icon.png");
    file_name_label = tgui::Label::create("No file currently selected");
    file_dialog_button = tgui::BitmapButton::create("Select file");

    file_dialog_button->setImage("resources/images/gui/folder_icon.png");

    tgui::HorizontalLayout::setSize(
        file_icon->getSize().x + file_name_label->getSize().x + file_dialog_button->getSize().x,
        file_icon->getSize().y
    );

/*     file_icon->setSize("20%", "100%");
    file_name_label->setSize("50%", "100%");
    file_dialog_button->setSize("30%", "100%"); */

    tgui::HorizontalLayout::add(file_icon, "FileIcon");
    tgui::HorizontalLayout::add(file_name_label, "FileNameLabel");
    tgui::HorizontalLayout::add(file_dialog_button, "FileDialogButton");
}

FileSelector::Ptr FileSelector::create(std::string file_path)
{
    FileSelector::Ptr file_selector = std::make_shared<FileSelector>();

    file_selector->setFile(file_path);

    return file_selector;
}

FileSelector::Ptr FileSelector::copy(FileSelector::ConstPtr widget)
{
    if (widget)
        return std::static_pointer_cast<FileSelector>(widget->clone());
    else
        return nullptr;
}

void FileSelector::setFile(std::string file_path)
{
    this->file_path = file_path;
}

tgui::Widget::Ptr FileSelector::clone() const
{
    return std::make_shared<FileSelector>(*this);
}
