#include "project.h"
#include <filesystem>
#include <fstream>
namespace Leaper
{

    void Project::Create(std::string_view path)
    {
        m_path              = path;
        m_asserts_directory = (std::filesystem::path(path).parent_path() / "asserts").string();
        std::filesystem::create_directory(m_asserts_directory);

        m_json["asserts"] = m_asserts_directory;
        // Output to files
        std::ofstream file(m_path);
        file << m_json.dump(4);
    }

    void Project::Open(std::string_view path)
    {
        m_path = path;
        std::ifstream file(m_path);
        m_json = nlohmann::json::parse(file);

        m_asserts_directory = m_json["asserts"];
    }

    void Project::AddScene(std::string_view path) {}

    void Project::AddScene(Scene* scene) {}

}  // namespace Leaper