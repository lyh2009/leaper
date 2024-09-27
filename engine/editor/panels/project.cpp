#include "function/render/texture.h"
#include "project.h"

#include <IconsFontAwesome6.h>
#include <imgui.h>
#include <imgui_internal.h>

#include <iostream>
namespace Leaper
{

    extern const std::filesystem::path g_assets_path = "assets";

    void Project::OnAttach()
    {
        m_path               = std::filesystem::path(g_assets_path);
        m_folder_icon        = Texture::Create("./resource/icons/folder.png");
        m_file_icon          = Texture::Create("./resource/icons/file.png");
        m_icon_map[".lua"]   = Texture::Create("./resource/icons/lua.png");
        m_icon_map[".png"]   = Texture::Create("./resource/icons/image.png");
        m_icon_map[".jpg"]   = Texture::Create("./resource/icons/image.png");
        m_icon_map[".blend"] = Texture::Create("./resource/icons/blend.png");
        m_icon_map[".obj"]   = Texture::Create("./resource/icons/blend.png");
        m_icon_map[".fbx"]   = Texture::Create("./resource/icons/blend.png");
        m_icon_map[".json"]  = Texture::Create("./resource/icons/json.png");
        m_icon_map[".mtl"]   = Texture::Create("./resource/icons/json.png");
        m_icon_map[".mp3"]   = Texture::Create("./resource/icons/audio.png");
    }

    void Project::OnUpdate()
    {
        ImGui::Begin(ICON_FA_TERMINAL "Project");
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

        static int icon_size = 60;
        static float padding = 16.0f;

        if (ImGui::BeginPopupContextItem("Setting"))
        {
            ImGui::SliderInt("Icon Size", &icon_size, 30, 80);
            ImGui::EndPopup();
        }

        if (m_path != std::filesystem::path(g_assets_path))
        {
            if (ImGui::Button("<-"))
            {
                m_path = m_path.parent_path();
            }
            ImGui::SameLine();
        }

        if (ImGui::Button(ICON_FA_BAHAI))
        {
            ImGui::OpenPopup("Setting");
        }
        ImGui::SameLine();
        ImGui::Text(m_path.string().c_str());

        ImGui::Columns(2);

        static bool init = true;
        if (init)
        {
            ImGui::SetColumnWidth(0, 240.0f);
            init = false;
        }

        if (ImGui::BeginChild("Folder"))
        {
            DrawTree();
        }
        ImGui::EndChild();

        ImGui::NextColumn();

        if (ImGui::BeginChild("File"))
        {
            DrawContent(icon_size);
        }
        ImGui::EndChild();

        ImGui::Columns(1);

        ImGui::End();
    }

    void Project::DrawTree()
    {
        for (auto& directory : std::filesystem::directory_iterator(m_path))
        {
            if (directory.is_directory())
            {
                ImGui::Image((void*)m_folder_icon->GetTexture(), ImVec2(20.0f, 20.0f), ImVec2(0, 1), ImVec2(1, 0));
                ImGui::SameLine();
                if (ImGui::Button(directory.path().filename().string().c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
                {
                    m_path /= directory.path().filename();
                }
            }
        }
    }

    void Project::DrawContent(int icon_size)
    {
        float cellSize   = icon_size + 16.0f;
        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columnCount  = (int)(panelWidth / cellSize);
        if (columnCount < 1)
            columnCount = 1;

        ImGui::Columns(columnCount, 0, false);

        for (auto& directory : std::filesystem::directory_iterator(m_path))
        {
            if (!directory.is_directory())
            {
                const auto& path = directory.path();
                auto extension   = path.extension();
                ImGui::PushID(directory.path().string().c_str());
                if (m_icon_map.find(extension) != m_icon_map.end())
                    ImGui::ImageButton((void*)m_icon_map.at(extension)->GetTexture(), ImVec2(icon_size, icon_size), ImVec2(0, 1), ImVec2(1, 0));
                else
                    ImGui::ImageButton((void*)m_file_icon->GetTexture(), ImVec2(icon_size, icon_size), ImVec2(0, 1), ImVec2(1, 0));

                if (ImGui::BeginDragDropSource())
                {
                    auto relative_path       = std::filesystem::relative(path, g_assets_path);
                    const wchar_t* item_path = relative_path.c_str();
                    ImGui::SetDragDropPayload("ASSETS", item_path, (wcslen(item_path) + 1) * sizeof(wchar_t));
                    ImGui::Text(relative_path.filename().string().c_str());
                    ImGui::EndDragDropSource();
                }
                ImGui::TextWrapped(directory.path().filename().string().c_str());
                ImGui::NextColumn();
                ImGui::PopID();
            }
        }
        ImGui::Columns(1);
    }
}  // namespace Leaper