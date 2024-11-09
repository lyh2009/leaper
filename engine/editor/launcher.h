#pragma once
#include "core/layer/layer.h"
#include "editor_layer.h"
#include "function/render/texture.h"
#include "project.h"

namespace Leaper
{
    class Launcher : public Layer
    {
    public:
        Launcher();
        virtual void OnAttach() override;
        virtual void OnUpdate() override;
        virtual void OnImGuiRender() override;

        virtual void OnEvent(Event& e) override;

    private:
        void DrawProjectCoontent(const std::string& project);

    private:
        EditorLayer* m_editor_layer = nullptr;
        bool b_is_open              = false;
        std::string m_file_path     = "./launcher.json";
        Ref<Texture> m_logo;

        nlohmann::json m_json;

        std::vector<Project> m_projects;
        std::vector<std::string> m_projects_paths;
    };
}  // namespace Leaper