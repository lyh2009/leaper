#include "content_broswer.h"
#include "core/log.h"
#include "function/render/texture.h"
#include "resource/gpu_resource_mapper.h"

#include <IconsFontAwesome6.h>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <imgui.h>
#include <imgui_internal.h>

#include <iostream>
#include <string>
#include <string_view>

#include "../global.h"

namespace Leaper
{

    void ContentBroswer::OnAttach(std::string_view assets_path)
    {
        m_path               = assets_path;
        m_assets_path        = assets_path;
        m_folder_icon        = TextureResourceManager::LoadTexture("./resource/icons/folder.png");
        m_file_icon          = TextureResourceManager::LoadTexture("./resource/icons/file.png");
        m_icon_map[".lua"]   = TextureResourceManager::LoadTexture("./resource/icons/lua.png");
        m_icon_map[".png"]   = TextureResourceManager::LoadTexture("./resource/icons/image.png");
        m_icon_map[".jpg"]   = TextureResourceManager::LoadTexture("./resource/icons/image.png");
        m_icon_map[".blend"] = TextureResourceManager::LoadTexture("./resource/icons/blend.png");
        m_icon_map[".obj"]   = TextureResourceManager::LoadTexture("./resource/icons/blend.png");
        m_icon_map[".fbx"]   = TextureResourceManager::LoadTexture("./resource/icons/blend.png");
        m_icon_map[".json"]  = TextureResourceManager::LoadTexture("./resource/icons/json.png");
        m_icon_map[".mtl"]   = TextureResourceManager::LoadTexture("./resource/icons/json.png");
        m_icon_map[".mp3"]   = TextureResourceManager::LoadTexture("./resource/icons/audio.png");

        LP_CORE_LOG_WARNING(m_path.string());
    }

    void ContentBroswer::OnImGuiRender()
    {
        ImGui::Begin(ICON_FA_FOLDER " ContentBroswer");
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

        static int icon_size = 60;
        static float padding = 16.0f;

        if (ImGui::BeginPopupContextItem("Setting"))
        {
            ImGui::SliderInt("Icon Size", &icon_size, 30, 80);
            ImGui::EndPopup();
        }

        if (m_path != m_assets_path)
        {
            if (ImGui::Button("<-"))
            {
                m_path = m_path.parent_path();
            }
            ImGui::SameLine();
        }
        ImGui::SameLine();
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

            if (ImGui::BeginPopupContextWindow(0, 1, false))
            {
                DrawMenu();
                ImGui::EndPopup();
            }
        }
        ImGui::EndChild();

        ImGui::Columns(1);

        ImGui::End();
    }

    void ContentBroswer::DrawMenu()
    {

        auto BaseMenu = [=](const std::string& lable, std::function<void(std::filesystem::path)> func) {
            if (ImGui::BeginMenu(lable.c_str()))
            {
                static std::filesystem::path new_dir;
                char buffer[256];
                memset(buffer, 0, sizeof(buffer));
                strncpy_s(buffer, sizeof(buffer), new_dir.filename().string().c_str(), sizeof(buffer));
                const std::string hint = "New " + lable + " Name";
                if (ImGui::InputTextWithHint("##Create", hint.c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_AutoSelectAll))
                    new_dir = m_path / std::string(buffer);
                ImGui::SameLine();
                if (ImGui::Button("Create"))
                    func(new_dir);

                ImGui::EndMenu();
            }
        };

        BaseMenu("Folder", [=](std::filesystem::path dir) { std::filesystem::create_directory(dir); });
        BaseMenu("Lua Script", [=](std::filesystem::path dir) {
            std::ofstream of(dir.string() + ".lua");
            of.close();
        });
    }

    void ContentBroswer::DrawTree()
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

    void ContentBroswer::DrawContent(int icon_size)
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
                    auto relative_path       = std::filesystem::relative(path, m_assets_path);
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