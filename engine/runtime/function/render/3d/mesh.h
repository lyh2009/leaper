#pragma once

#include "../buffers.h"
#include "../shader.h"
#include "../vertex_array.h"
#include "assimp/material.h"
#include "core/base.h"
#include "function/render/texture.h"
#include <cstdint>
#include <vector>

namespace Leaper
{
    enum class TextureType
    {
        Diffuse  = 0,
        Specular = 1,
        Normals  = 2,
    };

    class Mesh
    {
    public:
        struct MeshVertex
        {
            glm::vec3 position = { 0.0f, 0.0f, 0.0f };
            glm::vec3 normal   = { 0.0f, 0.0f, 0.0f };
            glm::vec2 texcoord = { 0.0f, 0.0f };
        };

        struct MeshTexture
        {
            Leaper::Ref<Texture> texture = nullptr;
            TextureType type;
        };

        Mesh(std::vector<MeshVertex> vertices, std::vector<uint32_t> indices, std::vector<MeshTexture> textures);
        void Render(glm::mat4& trans, glm::mat4& camera, glm::vec3 camera_pos);

    private:
        void Init();

        std::vector<MeshVertex> m_verteices;
        std::vector<uint32_t> m_indices;
        std::vector<MeshTexture> m_textures;
        Leaper::Ref<VertexBuffer> m_vertex_buffer;
        Leaper::Ref<VertexArray> m_vertex_array;
        Leaper::Ref<IndexBuffer> m_index_buffer;
        Leaper::Ref<Shader> m_shader;
    };
}  // namespace Leaper