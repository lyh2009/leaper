#pragma once
#include "function/render/texture.h"

#include <filesystem>

class Project
{
public:
    void OnAttach();
    void OnUpdate();
private:
    Leaper::Ref<Leaper::Texture> m_folder_icon;
    Leaper::Ref<Leaper::Texture> m_file_icon;

    std::filesystem::path m_current_directory;
    std::filesystem::path m_path;
};