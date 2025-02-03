#pragma once
#include "function/ecs/json.hpp"
#include <filesystem>
#include <function/ecs/json.hpp>
#include <function/ecs/scene.h>
#include <string_view>

namespace Leaper
{
    class Project
    {
    public:
        void Create(std::string_view path);
        void Open(std::string_view path);

        void AddScene(std::string_view path);
        void AddScene(Scene* scene);
        const std::string GetProjectPath() const
        {
            return m_path;
        }

    private:
        nlohmann::json m_json;
        std::string m_path;
        std::string m_asserts_directory;
    };
}  // namespace Leaper