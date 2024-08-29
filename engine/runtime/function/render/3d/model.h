#pragma once
#include "assimp/material.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"
#include "core/base.h"
#include "function/render/shader.h"
#include "function/render/texture.h"
#include "mesh.h"
#include <vector>

namespace Leaper
{
    class Model
    {
    public:
        Model() = default;
        Model(const std::string& path);
        void Draw(glm::mat4& trans, glm::mat4& camera, glm::vec3 camera_pos);

    private:
        void LoadModel(const std::string& path);
        void ProcessNode(aiNode* node, const aiScene* scene);
        Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Mesh::MeshTexture> LoadTexture(aiMaterial* mat, aiTextureType type, Leaper::TextureType type_name);

        std::vector<Mesh> m_meshes;
        Leaper::Ref<Shader> m_shader;
        std::string m_path;
    };
}  // namespace Leaper