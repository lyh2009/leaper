#include "core/events/events.h"
#include "editor_layer.h"
#include "function/application/application.h"
#include "function/render/texture.h"
#include "imgui.h"
#include "launcher.h"
#include <IconsFontAwesome6.h>
#include <filesystem>
#include <fstream>
#include <function/imgui/ui.h>
#include <function/utils/platform_utils.h>

namespace Leaper
{

    Launcher::Launcher() : Layer("LauncherLayer")
    {
        m_editor_layer = new EditorLayer();
    }
    void Launcher::OnAttach()
    {
        m_logo = Texture::Create("./resource/leaper.png");
        m_editor_layer->OnAttach();

        // Load launcher.json
        std::ifstream file(m_file_path);
        if (file.peek() == std::ifstream::traits_type::eof())
        {
            // Init file
            std::ofstream file(m_file_path);
            file << m_json.dump(4);
        }
        // Read file
        m_json = nlohmann::json::parse(file);
        if (!m_json["projects"].empty())
        {
            m_projects_paths = m_json["projects"];
        }
    }

    void Launcher::OnUpdate()
    {
        m_editor_layer->OnUpdate();
    }

    void Launcher::DrawProjectCoontent(const std::string& project)
    {
        // Get project name
        std::string::size_type iPos = project.find_last_of('\\') + 1;
        std::string filename        = project.substr(iPos, project.length() - iPos);
        std::string name            = filename.substr(0, filename.rfind("."));

        // Draw content
        ImGui::BeginChild(project.c_str(), ImVec2(0, 80), true);
        ImGui::Text(name.data());

        ImGui::BeginDisabled();
        ImGui::Text("%s", project.c_str());
        ImGui::EndDisabled();

        if (ImGui::Button("Edit"))
        {
            Application::Get().GetWindow()->SetTitle("Leaper Engine - " + name);
            m_editor_layer->InitPanels(std::filesystem::path(project).parent_path().string());
            b_is_open = true;
        }
        ImGui::SameLine();

        if (ImGui::Button("Remove"))
        {
            ImGui::OpenPopup("##Remove");
        }

        static const ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize;
        UI::SetNextWindowOnCenter();
        if (ImGui::BeginPopupModal("##Remove", NULL, flags))
        {
            ImGui::Text("Do you want to continue removing the program?");
            if (ImGui::Button("Remove"))
            {
                for (int i = 0; i < m_projects_paths.size(); i++)
                {
                    if (m_projects_paths[i] == project)
                    {
                        m_projects_paths.erase(m_projects_paths.begin() + i);
                        m_json["projects"] = m_projects_paths;
                        std::ofstream file(m_file_path);
                        file << m_json.dump(4);
                    }
                }
            }

            if (ImGui::Button("Cancel"))
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }

        ImGui::EndChild();
    }

    void Launcher::OnImGuiRender()
    {
        if (!b_is_open)
        {
            // Docking
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

            ImGui::Begin("Launcher", 0, window_flags);

            if (ImGui::Button("New"))
            {
                // Open a project
                Project project;
                std::string path = FileDialogs::SaveFile("Leaper Project(*.lpproject)\0*.lpproject\0");
                if (path != "")
                {
                    project.Create(path + ".lpproject");
                    m_projects.push_back(project);
                    m_projects_paths.push_back(project.GetProjectPath());

                    // Output to file
                    m_json["projects"] = m_projects_paths;
                    std::ofstream file(m_file_path);
                    file << m_json.dump(4);
                }
            }

            ImGui::SameLine();

            if (ImGui::Button("Add")) {}

            ImGui::BeginChild("ContentWindow");
            {
                for (int i = 0; i < m_projects_paths.size(); i++) { DrawProjectCoontent(m_projects_paths[i]); }
            }
            ImGui::EndChild();

            ImGui::End();
        }

        if (b_is_open)
        {
            m_editor_layer->OnImGuiRender();
        }
    }

    void Launcher::OnEvent(Event& e)
    {
        m_editor_layer->OnEvent(e);
    }

}  // namespace Leaper