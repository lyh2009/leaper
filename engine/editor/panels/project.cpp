#include "project.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <iostream>

extern const std::filesystem::path g_assets_path = "assets";

void Project::OnAttach()
{
    m_path = std::filesystem::path(g_assets_path);
    m_folder_icon = Leaper::Texture::Create("resource\\icons\\folder.png");
    m_file_icon = Leaper::Texture::Create("resource\\icons\\file.png");
}

void Project::OnUpdate()
{
    ImGui::Begin("Project");
    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    if (m_path != std::filesystem::path(g_assets_path))
	{
		if (ImGui::Button("<-"))
		{
			m_path = m_path.parent_path();
		}
        ImGui::SameLine();
	}
    
    if(ImGui::Button("..."))
    {
        ImGui::OpenPopup("Setting");
    }
    ImGui::SameLine();
    ImGui::Text(m_path.string().c_str());

    static int icon_size = 60;
    static float padding = 16.0f;
    if(ImGui::BeginPopupContextItem("Setting"))
    {
        ImGui::SliderInt("Icon Size", &icon_size, 30, 80);
        ImGui::EndPopup();
    }

    float cellSize = icon_size+ padding;

    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columnCount = (int)(panelWidth / cellSize);
    if (columnCount < 1)
        columnCount = 1;

    ImGui::Columns(columnCount, 0, false);

    for (auto &directory : std::filesystem::directory_iterator(m_path))
    {
        const auto& path = directory.path();

        Leaper::Ref<Leaper::Texture> icon = directory.is_directory() ? m_folder_icon : m_file_icon;
        ImGui::PushID(directory.path().string().c_str());

        ImGui::ImageButton((void *)icon->GetTexture(), ImVec2(icon_size, icon_size), ImVec2(0, 1), ImVec2(1, 0));

        if (ImGui::BeginDragDropSource())
        {
            auto relative_path = std::filesystem::relative(path, g_assets_path);
            const wchar_t *item_path = relative_path.c_str();
            ImGui::SetDragDropPayload("ASSETS", item_path, (wcslen(item_path) + 1) * sizeof(wchar_t));
            ImGui::Text(relative_path.filename().string().c_str());
            ImGui::EndDragDropSource();
        }


        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            if (directory.is_directory())
                m_path /= directory.path().filename();
        }

        ImGui::TextWrapped(directory.path().filename().string().c_str());

        ImGui::NextColumn();
        ImGui::PopID();
    }

    ImGui::Columns(1);
    ImGui::End();
}