#include "renderer2d.h"
#include "lppch.h"

#include "function/application/application.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <chrono>

Leaper::Ref<Leaper::RenderAPI> Leaper::Renderer2D::m_render_api = nullptr;
Leaper::OrthgraphicCamera Leaper::Renderer2D::m_camera = Leaper::OrthgraphicCamera(1.0f, -1.0f, 1.0f, -1.0f);
float Leaper::Renderer2D::m_line_width = 2.0f;
float Leaper::Renderer2D::m_flush_quad_elapsed_time = 0.0f;

struct QuadVertex
{
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 tex_coord;
    float tex_index;

    // editor only
    int entity_id = 1;
};

struct LineVertex
{
    glm::vec3 position;
    glm::vec4 color;

    int entity_id;
};

struct Renderer2DData
{
    static const uint32_t max_quads = 20000;
    static const uint32_t max_vertices = max_quads * 4;
    static const uint32_t max_indices = max_quads * 6;
    static const uint32_t max_texture_slots = 32;

    // quad
    uint32_t quad_index_count;
    Leaper::Ref<Leaper::VertexArray>  quad_vertex_array;
    Leaper::Ref<Leaper::VertexBuffer> quad_vertex_buffer;

    QuadVertex* quad_vertex_buffer_base = nullptr;
    QuadVertex* quad_vertex_buffer_ptr  = nullptr;

    Leaper::Ref<Leaper::Shader> quad_shader;

    // line
    uint32_t line_vertex_count;
    Leaper::Ref<Leaper::VertexArray>  line_vertex_array;
    Leaper::Ref<Leaper::VertexBuffer> line_vertex_buffer;

    LineVertex* line_vertex_buffer_base = nullptr;
    LineVertex* line_vertex_buffer_ptr  = nullptr;

    Leaper::Ref<Leaper::Shader> line_shader;

    std::array<Leaper::Ref<Leaper::Texture>, max_texture_slots> texture_slots;
	uint32_t texture_slot_index = 1; // 0 = white texture
    Leaper::Ref<Leaper::Texture> white_texture;
    glm::vec4 vertex_position[4];

    struct CameraData
	{
		glm::mat4 view_projection;
	};
	CameraData camera_buffer;
};

static Renderer2DData s_data;

void Leaper::Renderer2D::Init(Leaper::Ref<Leaper::RenderAPI> render_api)
{
    m_render_api = render_api;
    s_data.quad_shader = Leaper::Shader::Create("./resource/shaders/quad.vs", "./resource/shaders/quad.fs");
    s_data.line_shader = Leaper::Shader::Create("./resource/shaders/line.vs", "./resource/shaders/line.fs");

    // quad
    s_data.quad_vertex_array = Leaper::VertexArray::Create();
    s_data.quad_vertex_buffer = Leaper::VertexBuffer::Create(s_data.max_vertices * sizeof(QuadVertex));
    s_data.quad_vertex_buffer->SetLayout({
        { Leaper::ShaderDataType::Float3, "a_Position" },
        { Leaper::ShaderDataType::Float4, "a_Color"    },
        { Leaper::ShaderDataType::Float2, "a_TexCoord" },
        { Leaper::ShaderDataType::Float,  "a_TexIndex" },
        { Leaper::ShaderDataType::Int,    "a_EntityID" }
    });
    s_data.quad_vertex_array->AddVertexBuffer(s_data.quad_vertex_buffer);

    s_data.quad_vertex_buffer_base = new QuadVertex[Renderer2DData::max_vertices];

    uint32_t* quad_indices = new uint32_t[Renderer2DData::max_indices];
    uint32_t offset = 0;
    for(uint32_t i = 0; i < Renderer2DData::max_indices; i+=6)
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
    s_data.white_texture = Leaper::Texture::Create(1, 1);
    uint32_t white_texture_data = 0xffffffff;
    s_data.white_texture->SetData(&white_texture_data);
    s_data.texture_slots[0] = s_data.white_texture;

    s_data.vertex_position[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
    s_data.vertex_position[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
    s_data.vertex_position[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
    s_data.vertex_position[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

    // line
    s_data.line_vertex_array = Leaper::VertexArray::Create();
    s_data.line_vertex_buffer = Leaper::VertexBuffer::Create(s_data.max_vertices * sizeof(LineVertex));
    s_data.line_vertex_buffer->SetLayout({
        {Leaper::ShaderDataType::Float3, "a_Position"},
        {Leaper::ShaderDataType::Float4, "a_Color"   },
        {Leaper::ShaderDataType::Int,    "a_EntityID"}
    });

    s_data.line_vertex_array->AddVertexBuffer(s_data.line_vertex_buffer);

    s_data.line_vertex_buffer_base = new LineVertex[s_data.max_vertices];
}

void Leaper::Renderer2D::BeginScene(Leaper::OrthgraphicCamera camera)
{
    s_data.camera_buffer.view_projection = camera.GetViewProjectionMat();
    StartBatch();
}

void Leaper::Renderer2D::EndScene()
{
    Flush();
}

void Leaper::Renderer2D::StartBatch()
{
    s_data.quad_index_count = 0;
    s_data.quad_vertex_buffer_ptr = s_data.quad_vertex_buffer_base;

    s_data.line_vertex_count = 0;
    s_data.line_vertex_buffer_ptr = s_data.line_vertex_buffer_base;

    s_data.texture_slot_index = 1;
}

void Leaper::Renderer2D::Flush()
{
    auto start_time = std::chrono::high_resolution_clock::now();
    if(s_data.quad_index_count)
    {
        uint32_t data_size = (uint32_t)((uint8_t*)s_data.quad_vertex_buffer_ptr - (uint8_t*)s_data.quad_vertex_buffer_base);
        s_data.quad_vertex_buffer->SetData(s_data.quad_vertex_buffer_base, data_size);
        
        // bind texture
        for(uint32_t i = 0; i < s_data.texture_slot_index; i++)
            s_data.texture_slots[i]->Bind(i);

        s_data.quad_shader->Bind();
        s_data.quad_shader->SetMat4("u_ViewProjection", s_data.camera_buffer.view_projection);
        m_render_api->DrawElements(s_data.quad_vertex_array, s_data.quad_index_count);
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    m_flush_quad_elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count() * 0.01 * 0.01 * 0.01;

    if(s_data.line_vertex_count)
    {
        uint32_t data_size = (uint32_t)((uint8_t*)s_data.line_vertex_buffer_ptr - (uint8_t*)s_data.line_vertex_buffer_base);
        s_data.line_vertex_buffer->SetData(s_data.line_vertex_buffer_base, data_size);

        s_data.line_shader->Bind();
        SetLineWidth(m_line_width);
        s_data.line_shader->SetMat4("u_ViewProjection", s_data.camera_buffer.view_projection);
        m_render_api->DrawLines(s_data.line_vertex_array, s_data.line_vertex_count);
    }
    
}

void Leaper::Renderer2D::NextBatch()
{
    Flush();
    StartBatch();
}

void Leaper::Renderer2D::DrawQuad(Leaper::TransformComponent trans, glm::vec4 color, int entity_id)
{
    constexpr size_t quad_vertex_count = 4;
    const float texture_index = 0.0f; // white texture
	constexpr glm::vec2 texture_coords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

    if(s_data.quad_index_count >= Renderer2DData::max_indices)
        NextBatch();

    for(size_t i = 0; i < quad_vertex_count; i++)
    {
        s_data.quad_vertex_buffer_ptr->position = trans.GetTransform() * s_data.vertex_position[i];
        s_data.quad_vertex_buffer_ptr->tex_coord = texture_coords[i];
        s_data.quad_vertex_buffer_ptr->color = color;
        s_data.quad_vertex_buffer_ptr->tex_index = texture_index;
        s_data.quad_vertex_buffer_ptr->entity_id = entity_id;

        s_data.quad_vertex_buffer_ptr++;
    }

    s_data.quad_index_count += 6;
}

void Leaper::Renderer2D::DrawTexture(Leaper::TransformComponent trans, const Leaper::Ref<Leaper::Texture>& texture, glm::vec4 color, int entity_id)
{
    constexpr size_t quad_vertex_count = 4;
	constexpr glm::vec2 texture_coords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

    float texture_index = 0.0f; 
    // 判断纹理是否重复
    for(uint32_t i = 1; i < s_data.texture_slot_index; i++)
    {
        if(*s_data.texture_slots[i] == *texture)
        {
            texture_index = (float)i;
            break;
        }
    }

    if(texture_index == 0.0f)
    {
        if(s_data.texture_slot_index >= Renderer2DData::max_texture_slots)
            NextBatch();
        texture_index = (float)s_data.texture_slot_index;
        s_data.texture_slots[s_data.texture_slot_index] = texture;
        s_data.texture_slot_index++;
    }

    if(s_data.quad_index_count >= Renderer2DData::max_indices)
        NextBatch();

    for(uint32_t i = 0; i < quad_vertex_count; i++)
    {
        s_data.quad_vertex_buffer_ptr->position = trans.GetTransform() * s_data.vertex_position[i];
        s_data.quad_vertex_buffer_ptr->tex_coord = texture_coords[i];
        s_data.quad_vertex_buffer_ptr->color = color;
        s_data.quad_vertex_buffer_ptr->tex_index = texture_index;
        s_data.quad_vertex_buffer_ptr->entity_id = entity_id;

        s_data.quad_vertex_buffer_ptr++;
    }

    s_data.quad_index_count += 6;
}

void Leaper::Renderer2D::DrawTexture(Leaper::TransformComponent trans, Leaper::Animation2DComponent anim, const Leaper::Ref<Leaper::Texture> &texture, glm::vec4 color, int entity_id)
{
    constexpr size_t quad_vertex_count = 4;
	auto texture_coords = anim.texture_coords;

    float texture_index = 0.0f; 
    // 判断纹理是否重复
    for(uint32_t i = 1; i < s_data.texture_slot_index; i++)
    {
        if(*s_data.texture_slots[i] == *texture)
        {
            texture_index = (float)i;
            break;
        }
    }

    if(texture_index == 0.0f)
    {
        if(s_data.texture_slot_index >= Renderer2DData::max_texture_slots)
            NextBatch();
        texture_index = (float)s_data.texture_slot_index;
        s_data.texture_slots[s_data.texture_slot_index] = texture;
        s_data.texture_slot_index++;
    }

    if(s_data.quad_index_count >= Renderer2DData::max_indices)
        NextBatch();

    for(uint32_t i = 0; i < quad_vertex_count; i++)
    {
        s_data.quad_vertex_buffer_ptr->position = trans.GetTransform() * s_data.vertex_position[i];
        s_data.quad_vertex_buffer_ptr->tex_coord = texture_coords[i];
        s_data.quad_vertex_buffer_ptr->color = color;
        s_data.quad_vertex_buffer_ptr->tex_index = texture_index;
        s_data.quad_vertex_buffer_ptr->entity_id = entity_id;

        s_data.quad_vertex_buffer_ptr++;
    }

    s_data.quad_index_count += 6;
}

void Leaper::Renderer2D::DrawSprite(Leaper::TransformComponent trans, Leaper::Ref<Leaper::Texture> texture, glm::vec4 color, int entity_id)
{
    if(texture)
        Leaper::Renderer2D::DrawTexture(trans, texture, color, entity_id);
    else
        Leaper::Renderer2D::DrawQuad(trans, color, entity_id);
}

void Leaper::Renderer2D::DrawSprite(Leaper::TransformComponent trans, Leaper::Animation2DComponent anim, Leaper::Ref<Leaper::Texture> texture, glm::vec4 color, int entity_id)
{
    if(texture)
        Leaper::Renderer2D::DrawTexture(trans, anim, texture, color, entity_id);
    else
        Leaper::Renderer2D::DrawQuad(trans, color, entity_id);
}

void Leaper::Renderer2D::SetLineWidth(float width)
{
    m_line_width = width;
    m_render_api->SetLineWidth(width);
}

void Leaper::Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int entity_id)
{
    //if(s_data.line_vertex_count >= Renderer2DData::max_vertices)
      //  NextBatch();

    s_data.line_vertex_buffer_ptr->position = p0;
    s_data.line_vertex_buffer_ptr->color = color;
    s_data.line_vertex_buffer_ptr->entity_id = entity_id;
    s_data.line_vertex_buffer_ptr++;

    s_data.line_vertex_buffer_ptr->position = p1;
    s_data.line_vertex_buffer_ptr->color = color;
    s_data.line_vertex_buffer_ptr->entity_id = entity_id;
    s_data.line_vertex_buffer_ptr++;

    s_data.line_vertex_count += 2;
}

void Leaper::Renderer2D::DrawRect(Leaper::TransformComponent trans, const glm::vec4 &color, int entity_id)
{
    glm::vec3 line_vertices[4];
    for(size_t i = 0; i < 4; i++)
        line_vertices[i] = trans.GetTransform() * s_data.vertex_position[i];
    
    DrawLine(line_vertices[0], line_vertices[1], color, entity_id);
    DrawLine(line_vertices[1], line_vertices[2], color, entity_id);
    DrawLine(line_vertices[2], line_vertices[3], color, entity_id);
    DrawLine(line_vertices[3], line_vertices[0], color, entity_id);
}
