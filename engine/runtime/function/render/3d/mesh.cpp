#include "function/render/renderer2d.h"
#include "function/render/renderer3d.h"
#include "function/render/uniform_buffer.h"
#include "resource/gpu_resource_mapper.h"
#include "resource/shader_library.h"
#include <vector>
#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NOEXCEPTION

#include "../render_command.h"
#include "core/log.h"
#include "function/render/buffers.h"
#include "mesh.h"
#include <cstdint>

Leaper::Mesh::Mesh(std::vector<MeshVertex> vertices, std::vector<uint32_t> indices, std::vector<Mesh::MeshTexture> textures)
    : m_verteices(vertices), m_indices(indices), m_textures(textures)
{
    m_vertex_array = Leaper::VertexArray::Create();
    m_vertex_array->Bind();
    m_vertex_buffer = Leaper::VertexBuffer::Create(vertices.size() * sizeof(MeshVertex));
    m_vertex_buffer->SetLayout(
        { { Leaper::ShaderDataType::Float3, "a_Position" }, { Leaper::ShaderDataType::Float3, "a_Normal" }, { Leaper::ShaderDataType::Float2, "a_TexCoord" } });
    m_vertex_buffer->SetData(m_verteices.data(), sizeof(MeshVertex) * m_verteices.size());

    m_vertex_array->AddVertexBuffer(m_vertex_buffer);

    m_index_buffer = Leaper::IndexBuffer::Create(m_indices.data(), indices.size() * sizeof(uint32_t));
    m_vertex_array->SetIndexBuffer(m_index_buffer);

    m_white_texture = TextureResourceManager::GetWhiteTexture();
}

void Leaper::Mesh::Render(glm::mat4& trans, Ref<Shader> shader, int entity_id)
{

    if (m_textures.empty())
        m_white_texture->Bind(1);
    else
    {
        for (int i = 0; i < m_textures.size(); i++)
        {
            if (m_textures[i].type == TextureType::Diffuse)
                m_textures[i].texture->Bind(1);
            else if (m_textures[i].type == TextureType::Specular)
                m_textures[i].texture->Bind(2);
        }
    }

    shader->Bind();

    Init();
    shader->SetMat4("u_Trans", trans);
    shader->SetInt("u_EntityID", entity_id);
    RenderCommand::DrawElements(m_vertex_array, static_cast<unsigned int>(m_indices.size()));
}

void Leaper::Mesh::Init() {}
