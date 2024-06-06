#pragma once

#include <string>
#include <memory>
#include <TGUI/Widgets/HorizontalLayout.hpp>
#include <TGUI/Widgets/Picture.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/BitmapButton.hpp>

class FileSelector : public tgui::HorizontalLayout
{
public:
    typedef std::shared_ptr<FileSelector> Ptr;
    typedef std::shared_ptr<const FileSelector> ConstPtr;

    FileSelector(const char *typeName = "FileSelector", bool initRenderer = true);
    ~FileSelector() = default;

    static FileSelector::Ptr create(std::string file_path = "");
    static FileSelector::Ptr copy(FileSelector::ConstPtr widget);

    void setFile(std::string file_path);

protected:
    tgui::Widget::Ptr clone() const override;

private:
    tgui::Picture::Ptr file_icon;
    tgui::Label::Ptr file_name_label;
    tgui::BitmapButton::Ptr file_dialog_button;

    std::string file_path;
};
