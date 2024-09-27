#include "buffers.h"
#include "core/base.h"
#include "function/ecs/components.h"
#include "glm/fwd.hpp"
#include "lppch.h"
#include "renderer2d.h"

#include "perspective_camera.h"
#include "perspective_camera_controller.h"
#include "render_command.h"
#include "shader.h"
#include "vertex_array.h"

#include <cstddef>
#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>
#include <chrono>
#include <string>

glm::vec3 Leaper::Renderer2D::m_ambient_light       = glm::vec3(0.3f, 0.3f, 0.3f);
float Leaper::Renderer2D::m_line_width              = 2.0f;
float Leaper::Renderer2D::m_flush_quad_elapsed_time = 0.0f;

struct QuadVertex
{
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 tex_coord;
    float tex_index;

    // editor only
    int entity_id;
};

struct LineVertex
{
    glm::vec3 position;
    glm::vec4 color;

    int entity_id;
};

struct CircleVertex
{
    glm::vec3 world_position;
    glm::vec3 local_position;
    glm::vec4 color;
    float thickness;
    float fade;

    int entity_id;
};

struct PointLight
{
    glm::vec3 position;
    glm::vec3 color;
    float intensity = 0.0f;
};

struct Renderer2DData
{
    static const uint32_t max_quads         = 20000;
    static const uint32_t max_vertices      = max_quads * 4;
    static const uint32_t max_indices       = max_quads * 6;
    static const uint32_t max_texture_slots = 32;

    // quad
    uint32_t quad_index_count;
    Leaper::Ref<Leaper::VertexArray> quad_vertex_array;
    Leaper::Ref<Leaper::VertexBuffer> quad_vertex_buffer;

    QuadVertex* quad_vertex_buffer_base = nullptr;
    QuadVertex* quad_vertex_buffer_ptr  = nullptr;

    Leaper::Ref<Leaper::Shader> quad_shader;

    // line
    uint32_t line_index_count;
    Leaper::Ref<Leaper::VertexArray> line_vertex_array;
    Leaper::Ref<Leaper::VertexBuffer> line_vertex_buffer;

    LineVertex* line_vertex_buffer_base = nullptr;
    LineVertex* line_vertex_buffer_ptr  = nullptr;

    Leaper::Ref<Leaper::Shader> line_shader;

    // circle
    uint32_t circle_index_count;
    Leaper::Ref<Leaper::VertexArray> circle_vertex_array;
    Leaper::Ref<Leaper::VertexBuffer> circle_vertex_buffer;

    CircleVertex* circle_vertex_buffer_base = nullptr;
    CircleVertex* circle_vertex_buffer_ptr  = nullptr;

    Leaper::Ref<Leaper::Shader> circle_shader;

    // light
    PointLight point_light[4];
    int light_count = 0;

    std::array<Leaper::Ref<Leaper::Texture>, max_texture_slots> texture_slots;
    uint32_t texture_slot_index = 1;  // 0 = white texture
    Leaper::Ref<Leaper::Texture> white_texture;
    glm::vec4 vertex_position[4];

    Leaper::Renderer2D::Statistics stats;

    struct CameraData
    {
        glm::mat4 view_projection;
    };
    CameraData camera_buffer;
};

static Renderer2DData s_data;

void Leaper::Renderer2D::Init()
{
    s_data.quad_shader   = Leaper::Shader::Create("./resource/shaders/quad.vs", "./resource/shaders/quad.fs");
    s_data.line_shader   = Leaper::Shader::Create("./resource/shaders/line.vs", "./resource/shaders/line.fs");
    s_data.circle_shader = Leaper::Shader::Create("./resource/shaders/circle.vs", "./resource/shaders/circle.fs");

    // quad
    s_data.quad_vertex_array  = Leaper::VertexArray::Create();
    s_data.quad_vertex_buffer = Leaper::VertexBuffer::Create(s_data.max_vertices * sizeof(QuadVertex));
    s_data.quad_vertex_buffer->SetLayout({ { Leaper::ShaderDataType::Float3, "a_Position" },
                                           { Leaper::ShaderDataType::Float4, "a_Color" },
                                           { Leaper::ShaderDataType::Float2, "a_TexCoord" },
                                           { Leaper::ShaderDataType::Float, "a_TexIndex" },
                                           { Leaper::ShaderDataType::Int, "a_EntityID" } });
    s_data.quad_vertex_array->AddVertexBuffer(s_data.quad_vertex_buffer);

    s_data.quad_vertex_buffer_base = new QuadVertex[Renderer2DData::max_vertices];

    uint32_t* quad_indices = new uint32_t[Renderer2DData::max_indices];
    uint32_t offset        = 0;
    for (uint32_t i = 0; i < Renderer2DData::max_indices; i += 6)
    {
        quad_indices[i + 0] = offset + 0;
        quad_indices[i + 1] = offset + 1;
        quad_indices[i + 2] = offset + 2;

        quad_indices[i + 3] = offset + 2;
        quad_indices[i + 4] = offset + 3;
        quad_indices[i + 5] = offset + 0;

        offset += 4;
    }

    Leaper::Ref<Leaper::IndexBuffer> quad_index_buffer = Leaper::IndexBuffer::Create(quad_indices, Renderer2DData::max_indices);
    s_data.quad_vertex_array->SetIndexBuffer(quad_index_buffer);
    delete[] quad_indices;

    // white texture
    s_data.white_texture        = Leaper::Texture::Create(1, 1);
    uint32_t white_texture_data = 0xffffffff;
    s_data.white_texture->SetData(&white_texture_data);
    s_data.texture_slots[0] = s_data.white_texture;

    s_data.vertex_position[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
    s_data.vertex_position[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
    s_data.vertex_position[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
    s_data.vertex_position[3] = { -0.5f, 0.5f, 0.0f, 1.0f };

    // line
    {
        s_data.line_vertex_array  = Leaper::VertexArray::Create();
        s_data.line_vertex_buffer = Leaper::VertexBuffer::Create(s_data.max_vertices * sizeof(LineVertex));
        s_data.line_vertex_buffer->SetLayout(
            { { Leaper::ShaderDataType::Float3, "a_Position" }, { Leaper::ShaderDataType::Float4, "a_Color" }, { Leaper::ShaderDataType::Int, "a_EntityID" } });

        s_data.line_vertex_array->AddVertexBuffer(s_data.line_vertex_buffer);

        s_data.line_vertex_buffer_base = new LineVertex[s_data.max_vertices];
    }

    // circle
    {
        s_data.circle_vertex_array  = Leaper::VertexArray::Create();
        s_data.circle_vertex_buffer = Leaper::VertexBuffer::Create(s_data.max_vertices * sizeof(CircleVertex));
        s_data.circle_vertex_buffer->SetLayout({ { Leaper::ShaderDataType::Float3, "a_WorldPosition" },
                                                 { Leaper::ShaderDataType::Float3, "a_LocalPosition" },
                                                 { Leaper::ShaderDataType::Float4, "a_Color" },
                                                 { Leaper::ShaderDataType::Float, "a_Thickness" },
                                                 { Leaper::ShaderDataType::Float, "a_Fade" },
                                                 { Leaper::ShaderDataType::Int, "a_EntityID" } });

        s_data.circle_vertex_array->AddVertexBuffer(s_data.circle_vertex_buffer);
        s_data.circle_vertex_array->SetIndexBuffer(quad_index_buffer);
        s_data.circle_vertex_buffer_base = new CircleVertex[s_data.max_vertices];
    }
}

void Leaper::Renderer2D::BeginScene(const glm::mat4& camera)
{
    s_data.camera_buffer.view_projection = camera;
    StartBatch();
}

void Leaper::Renderer2D::BeginScene(const glm::mat4& camera, glm::mat4& trans)
{
    s_data.camera_buffer.view_projection = camera * trans;
    StartBatch();
}

void Leaper::Renderer2D::EndScene()
{
    Flush();
}

void Leaper::Renderer2D::StartBatch()
{
    s_data.quad_index_count       = 0;
    s_data.quad_vertex_buffer_ptr = s_data.quad_vertex_buffer_base;

    s_data.line_index_count       = 0;
    s_data.line_vertex_buffer_ptr = s_data.line_vertex_buffer_base;

    s_data.circle_index_count       = 0;
    s_data.circle_vertex_buffer_ptr = s_data.circle_vertex_buffer_base;

    s_data.texture_slot_index = 1;
}

void Leaper::Renderer2D::Flush()
{
    // quad
    auto start_time = std::chrono::high_resolution_clock::now();
    if (s_data.quad_index_count)
    {
        uint32_t data_size = (uint32_t)((uint8_t*)s_data.quad_vertex_buffer_ptr - (uint8_t*)s_data.quad_vertex_buffer_base);
        s_data.quad_vertex_buffer->SetData(s_data.quad_vertex_buffer_base, data_size);

        // bind texture
        for (uint32_t i = 0; i < s_data.texture_slot_index; i++) s_data.texture_slots[i]->Bind(i);

        s_data.quad_shader->Bind();

        s_data.quad_shader->SetMat4("u_ViewProjection", s_data.camera_buffer.view_projection);
        s_data.quad_shader->SetVec3("u_AmbientLight", m_ambient_light);

        // point light
        for (int i = 0; i < 4; i++)
        {
            s_data.quad_shader->SetVec3("u_PointLight[" + std::to_string(i) + "].position", s_data.point_light[i].position);
            s_data.quad_shader->SetVec3("u_PointLight[" + std::to_string(i) + "].color", s_data.point_light[i].color);
            s_data.quad_shader->SetFloat("u_PointLight[" + std::to_string(i) + "].intensity", s_data.point_light[i].intensity);
        }

        Leaper::RenderCommand::DrawElements(s_data.quad_vertex_array, s_data.quad_index_count);

        s_data.quad_shader->UnBind();
        for (uint32_t i = 0; i < s_data.texture_slot_index; i++) s_data.texture_slots[i]->UnBind();
    }
    auto end_time             = std::chrono::high_resolution_clock::now();
    m_flush_quad_elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count() * 0.01 * 0.01 * 0.01;

    // line
    if (s_data.line_index_count)
    {
        uint32_t data_size = (uint32_t)((uint8_t*)s_data.line_vertex_buffer_ptr - (uint8_t*)s_data.line_vertex_buffer_base);
        s_data.line_vertex_buffer->SetData(s_data.line_vertex_buffer_base, data_size);

        s_data.line_shader->Bind();
        SetLineWidth(m_line_width);
        s_data.line_shader->SetMat4("u_ViewProjection", s_data.camera_buffer.view_projection);
        Leaper::RenderCommand::DrawLines(s_data.line_vertex_array, s_data.line_index_count);
        s_data.line_shader->UnBind();
    }

    // circle
    if (s_data.circle_index_count)
    {
        uint32_t data_size = (uint32_t)((uint8_t*)s_data.circle_vertex_buffer_ptr - (uint8_t*)s_data.circle_vertex_buffer_base);
        s_data.circle_vertex_buffer->SetData(s_data.circle_vertex_buffer_base, data_size);

        s_data.circle_shader->Bind();

        s_data.circle_shader->SetMat4("u_ViewProjection", s_data.camera_buffer.view_projection);
        Leaper::RenderCommand::DrawElements(s_data.circle_vertex_array, s_data.circle_index_count);
        s_data.circle_shader->UnBind();
    }
}

void Leaper::Renderer2D::NextBatch()
{
    Flush();
    StartBatch();
}

void Leaper::Renderer2D::DrawQuad(Leaper::TransformComponent trans, glm::vec4 color, int entity_id)
{
    constexpr size_t quad_vertex_count   = 4;
    const float texture_index            = 0.0f;  // white texture
    constexpr glm::vec2 texture_coords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

    if (s_data.quad_index_count >= Renderer2DData::max_indices)
        NextBatch();

    for (size_t i = 0; i < quad_vertex_count; i++)
    {
        s_data.quad_vertex_buffer_ptr->position  = trans.GetTransform() * s_data.vertex_position[i];
        s_data.quad_vertex_buffer_ptr->tex_coord = texture_coords[i];
        s_data.quad_vertex_buffer_ptr->color     = color;
        s_data.quad_vertex_buffer_ptr->tex_index = texture_index;
        s_data.quad_vertex_buffer_ptr->entity_id = entity_id;

        s_data.quad_vertex_buffer_ptr++;
    }

    s_data.quad_index_count += 6;
}

void Leaper::Renderer2D::DrawTexture(Leaper::TransformComponent trans, const Leaper::Ref<Leaper::Texture>& texture, glm::vec4 color, int entity_id)
{
    constexpr size_t quad_vertex_count   = 4;
    constexpr glm::vec2 texture_coords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

    float texture_index = 0.0f;
    // 判断纹理是否重复
    for (uint32_t i = 1; i < s_data.texture_slot_index; i++)
    {
        if (*s_data.texture_slots[i] == *texture)
        {
            texture_index = (float)i;
            break;
        }
    }

    if (texture_index == 0.0f)
    {
        if (s_data.texture_slot_index >= Renderer2DData::max_texture_slots)
            NextBatch();
        texture_index                                   = (float)s_data.texture_slot_index;
        s_data.texture_slots[s_data.texture_slot_index] = texture;
        s_data.texture_slot_index++;
    }

    if (s_data.quad_index_count >= Renderer2DData::max_indices)
        NextBatch();

    for (uint32_t i = 0; i < quad_vertex_count; i++)
    {
        s_data.quad_vertex_buffer_ptr->position  = trans.GetTransform() * s_data.vertex_position[i];
        s_data.quad_vertex_buffer_ptr->tex_coord = texture_coords[i];
        s_data.quad_vertex_buffer_ptr->color     = color;
        s_data.quad_vertex_buffer_ptr->tex_index = texture_index;
        s_data.quad_vertex_buffer_ptr->entity_id = entity_id;

        s_data.quad_vertex_buffer_ptr++;
    }

    s_data.quad_index_count += 6;
}

void Leaper::Renderer2D::DrawTexture(Leaper::TransformComponent trans, Leaper::Animation2DComponent anim, const Leaper::Ref<Leaper::Texture>& texture, glm::vec4 color,
                                     int entity_id)
{
    constexpr size_t quad_vertex_count = 4;
    auto texture_coords                = anim.texture_coords;

    float texture_index = 0.0f;
    // 判断纹理是否重复
    for (uint32_t i = 1; i < s_data.texture_slot_index; i++)
    {
        if (*s_data.texture_slots[i] == *texture)
        {
            texture_index = (float)i;
            break;
        }
    }

    if (texture_index == 0.0f)
    {
        if (s_data.texture_slot_index >= Renderer2DData::max_texture_slots)
            NextBatch();
        texture_index                                   = (float)s_data.texture_slot_index;
        s_data.texture_slots[s_data.texture_slot_index] = texture;
        s_data.texture_slot_index++;
    }

    if (s_data.quad_index_count >= Renderer2DData::max_indices)
        NextBatch();

    for (uint32_t i = 0; i < quad_vertex_count; i++)
    {
        s_data.quad_vertex_buffer_ptr->position  = trans.GetTransform() * s_data.vertex_position[i];
        s_data.quad_vertex_buffer_ptr->tex_coord = texture_coords[i];
        s_data.quad_vertex_buffer_ptr->color     = color;
        s_data.quad_vertex_buffer_ptr->tex_index = texture_index;
        s_data.quad_vertex_buffer_ptr->entity_id = entity_id;

        s_data.quad_vertex_buffer_ptr++;
    }

    s_data.quad_index_count += 6;
}

void Leaper::Renderer2D::DrawLight(Leaper::TransformComponent trans, Leaper::LightComponent light)
{
    s_data.point_light[light.id].position  = trans.position;
    s_data.point_light[light.id].color     = light.color;
    s_data.point_light[light.id].intensity = light.intensity;
}

void Leaper::Renderer2D::DrawSprite(Leaper::TransformComponent trans, Leaper::Ref<Leaper::Texture> texture, glm::vec4 color, int entity_id)
{
    if (texture)
        Leaper::Renderer2D::DrawTexture(trans, texture, color, entity_id);
    else
        Leaper::Renderer2D::DrawQuad(trans, color, entity_id);
}

void Leaper::Renderer2D::DrawSprite(Leaper::TransformComponent trans, Leaper::Animation2DComponent anim, Leaper::Ref<Leaper::Texture> texture, glm::vec4 color, int entity_id)
{
    if (texture)
        Leaper::Renderer2D::DrawTexture(trans, anim, texture, color, entity_id);
    else
        Leaper::Renderer2D::DrawQuad(trans, color, entity_id);
}

void Leaper::Renderer2D::SetLineWidth(float width)
{
    m_line_width = width;
    Leaper::RenderCommand::SetLineWidth(width);
}

void Leaper::Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int entity_id)
{
    // if(s_data.line_index_count >= Renderer2DData::max_vertices)
    //   NextBatch();

    s_data.line_vertex_buffer_ptr->position  = p0;
    s_data.line_vertex_buffer_ptr->color     = color;
    s_data.line_vertex_buffer_ptr->entity_id = entity_id;
    s_data.line_vertex_buffer_ptr++;

    s_data.line_vertex_buffer_ptr->position  = p1;
    s_data.line_vertex_buffer_ptr->color     = color;
    s_data.line_vertex_buffer_ptr->entity_id = entity_id;
    s_data.line_vertex_buffer_ptr++;

    s_data.line_index_count += 2;
}

void Leaper::Renderer2D::DrawRect(Leaper::TransformComponent trans, const glm::vec4& color, int entity_id)
{
    glm::vec3 line_vertices[4];
    for (size_t i = 0; i < 4; i++) line_vertices[i] = trans.GetTransform() * s_data.vertex_position[i];

    DrawLine(line_vertices[0], line_vertices[1], color, entity_id);
    DrawLine(line_vertices[1], line_vertices[2], color, entity_id);
    DrawLine(line_vertices[2], line_vertices[3], color, entity_id);
    DrawLine(line_vertices[3], line_vertices[0], color, entity_id);
}

void Leaper::Renderer2D::DrawCircle(Leaper::TransformComponent trans, const glm::vec4& color, float thickness, float fade, int entity_id)
{
    for (size_t i = 0; i < 4; i++)
    {
        s_data.circle_vertex_buffer_ptr->world_position = trans.GetTransform() * s_data.vertex_position[i];
        s_data.circle_vertex_buffer_ptr->local_position = s_data.vertex_position[i] * 2.0f;
        s_data.circle_vertex_buffer_ptr->color          = color;
        s_data.circle_vertex_buffer_ptr->thickness      = thickness;
        s_data.circle_vertex_buffer_ptr->fade           = fade;
        s_data.circle_vertex_buffer_ptr->entity_id      = entity_id;

        s_data.circle_vertex_buffer_ptr++;
    }

    s_data.circle_index_count += 6;
}

void Leaper::Renderer2D::ResetStats()
{
    memset(&s_data.stats, 0, sizeof(Leaper::Renderer2D::Statistics));
}

Leaper::Renderer2D::Statistics Leaper::Renderer2D::GetStats()
{
    return s_data.stats;
}