#include "renderer3d.h"

#include "3d/model.h"
#include "buffers.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/matrix.hpp"
#include "render_command.h"
#include "shader.h"
#include "texture.h"
#include "vertex_array.h"

#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include <array>

struct CubeVertex
{
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 tex_coord;
    glm::vec3 normal;

    float tex_index;

    // editor only
    int entity_id;
};

struct Renderer3DData
{
    static const uint32_t max_cubes         = 20000;
    static const uint32_t max_vertices      = max_cubes * 8;
    static const uint32_t max_indices       = max_cubes * 36;
    static const uint32_t max_texture_slots = 32;

    uint32_t cube_index_count;
    Leaper::Ref<Leaper::VertexArray> cube_vertex_array;
    Leaper::Ref<Leaper::VertexBuffer> cube_vertex_buffer;

    CubeVertex* cube_vertex_buffer_base = nullptr;
    CubeVertex* cube_vertex_buffer_ptr  = nullptr;

    Leaper::Ref<Leaper::Shader> cube_shader;

    std::array<Leaper::Ref<Leaper::Texture>, max_texture_slots> texture_slots;
    uint32_t texture_slot_index = 1;  // 0 = white texture
    Leaper::Ref<Leaper::Texture> white_texture;

    glm::vec4 cube_vertex_position[36]{ glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f), glm::vec4(0.5f, -0.5f, -0.5f, 1.0f), glm::vec4(0.5f, 0.5f, -0.5f, 1.0f),  // 后
                                        glm::vec4(0.5f, 0.5f, -0.5f, 1.0f),   glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f), glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f),

                                        glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f),  glm::vec4(0.5f, -0.5f, 0.5f, 1.0f),  glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),  // 前
                                        glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),    glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f),  glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f),

                                        glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f),   glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f), glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f),  // 左
                                        glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f), glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f), glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f),

                                        glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),    glm::vec4(0.5f, 0.5f, -0.5f, 1.0f),  glm::vec4(0.5f, -0.5f, -0.5f, 1.0f),  // 右
                                        glm::vec4(0.5f, -0.5f, -0.5f, 1.0f),  glm::vec4(0.5f, -0.5f, 0.5f, 1.0f),  glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),

                                        glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f), glm::vec4(0.5f, -0.5f, -0.5f, 1.0f), glm::vec4(0.5f, -0.5f, 0.5f, 1.0f),  // 下
                                        glm::vec4(0.5f, -0.5f, 0.5f, 1.0f),   glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f), glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f),

                                        glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f),  glm::vec4(0.5f, 0.5f, -0.5f, 1.0f),  glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),  // 上
                                        glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),    glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f),  glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f) };

    glm::vec3 cube_normal[36]{ glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f),
                               glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),
                               glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f),
                               glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3(1.0f, 0.0f, 0.0f),
                               glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3(0.0f, -1.0f, 0.0f),
                               glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f),
                               glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f),
                               glm::vec3(0.0f, 1.0f, 0.0f) };

    struct CameraBuffer
    {
        glm::mat4 view_projection;
        glm::vec3 position;
    };

    CameraBuffer camera_buffer;
};

static Renderer3DData s_data;

void Leaper::Renderer3D::Init()
{
    s_data.cube_shader = Leaper::Shader::Create("./resource/shaders/3d/cube.vs", "./resource/shaders/3d/cube.fs");

    s_data.cube_vertex_array  = Leaper::VertexArray::Create();
    s_data.cube_vertex_buffer = Leaper::VertexBuffer::Create(s_data.max_vertices * sizeof(CubeVertex));
    s_data.cube_vertex_buffer->SetLayout({ { Leaper::ShaderDataType::Float3, "a_Position" },
                                           { Leaper::ShaderDataType::Float4, "a_Color" },
                                           { Leaper::ShaderDataType::Float2, "a_TexCoord" },
                                           { Leaper::ShaderDataType::Float, "a_TexIndex" },
                                           { Leaper::ShaderDataType::Float3, "a_Normal" },
                                           { Leaper::ShaderDataType::Int, "a_EntityID" } });
    s_data.cube_vertex_array->AddVertexBuffer(s_data.cube_vertex_buffer);

    s_data.cube_vertex_buffer_base = new CubeVertex[Renderer3DData::max_vertices];

    s_data.white_texture        = Leaper::Texture::Create(1, 1);
    uint32_t white_texture_data = 0xffffffff;
    s_data.white_texture->SetData(&white_texture_data);
    s_data.texture_slots[0] = s_data.white_texture;
}

void Leaper::Renderer3D::BeginScene(const glm::mat4& camera, glm::vec3 position)
{
    s_data.camera_buffer.view_projection = camera;
    s_data.camera_buffer.position        = position;
    StartBatch();
}

void Leaper::Renderer3D::BeginScene(const glm::mat4& camera, glm::mat4& trans)
{
    s_data.camera_buffer.view_projection = camera * trans;
    StartBatch();
}

void Leaper::Renderer3D::EndScene()
{
    Flush();
}

void Leaper::Renderer3D::StartBatch()
{
    s_data.cube_index_count       = 0;
    s_data.cube_vertex_buffer_ptr = s_data.cube_vertex_buffer_base;

    s_data.texture_slot_index = 1;
}

void Leaper::Renderer3D::Flush()
{
    if (s_data.cube_index_count)
    {
        uint32_t data_size = (uint32_t)((uint8_t*)s_data.cube_vertex_buffer_ptr - (uint8_t*)s_data.cube_vertex_buffer_base);
        s_data.cube_vertex_buffer->SetData(s_data.cube_vertex_buffer_base, data_size);

        // bind texture
        for (uint32_t i = 0; i < s_data.texture_slot_index; i++) s_data.texture_slots[i]->Bind(i);

        s_data.cube_shader->Bind();

        s_data.cube_shader->SetMat4("u_ViewProjection", s_data.camera_buffer.view_projection);
        s_data.cube_shader->SetVec3("u_ViewPosition", s_data.camera_buffer.position);

        s_data.cube_vertex_array->DrawArray(s_data.max_vertices);
    }
}

void Leaper::Renderer3D::NextBatch()
{
    Flush();
    StartBatch();
}

void Leaper::Renderer3D::DrawCube(glm::mat4& trans, const Leaper::Ref<Leaper::Texture>& texture, glm::vec4 color, int entity_id)
{
    constexpr size_t cube_vertex_count   = 36;
    constexpr glm::vec2 texture_coords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

    float texture_index = 0.0f;

    if (s_data.cube_index_count >= Renderer3DData::max_indices) NextBatch();

    for (uint32_t i = 0; i < cube_vertex_count; i++)
    {
        s_data.cube_vertex_buffer_ptr->position  = trans * s_data.cube_vertex_position[i];
        s_data.cube_vertex_buffer_ptr->color     = color;
        s_data.cube_vertex_buffer_ptr->tex_coord = texture_coords[i];
        s_data.cube_vertex_buffer_ptr->tex_index = texture_index;
        s_data.cube_vertex_buffer_ptr->entity_id = entity_id;
        s_data.cube_vertex_buffer_ptr->normal    = s_data.cube_normal[i] * glm::mat3(glm::transpose(glm::inverse(trans)));

        s_data.cube_vertex_buffer_ptr++;
    }

    s_data.cube_index_count += 36;
}

void Leaper::Renderer3D::DrawModel(Model model, glm::mat4& trans, int entity_id)
{
    model.Draw(trans, s_data.camera_buffer.view_projection, s_data.camera_buffer.position, entity_id);
}
