#pragma once
#include "function/render/texture.h"

#include <filesystem>
#include <string>
#include <unordered_map>
namespace Leaper
{

    class ContentBroswer
    {
    public:
        void OnAttach(std::string_view assets_path);
        void OnImGuiRender();

    private:
        void DrawMenu();
        void DrawTree();
        void DrawContent(int icon_size);

        Leaper::Ref<Leaper::Texture> m_folder_icon;
        Leaper::Ref<Leaper::Texture> m_file_icon;
        Leaper::Ref<Leaper::Texture> m_lua_icon;
        Leaper::Ref<Leaper::Texture> m_image_icon;
        Leaper::Ref<Leaper::Texture> m_obj_icon;
        Leaper::Ref<Leaper::Texture> m_json_icon;
        Leaper::Ref<Leaper::Texture> m_audio_icon;
        Leaper::Ref<Leaper::Texture> m_icon = nullptr;

        std::unordered_map<std::filesystem::path, Leaper::Ref<Leaper::Texture>> m_icon_map;

        std::filesystem::path m_current_directory;
        std::filesystem::path m_path;
        std::filesystem::path m_assets_path;
    };
}  // namespace Leaper