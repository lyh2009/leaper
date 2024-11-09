#include "renderer3d.h"

#include "3d/model.h"
#include "buffers.h"
#include "function/ecs/components.h"
#include "function/render/framebuffer.h"
#include "function/render/uniform_buffer.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/matrix.hpp"
#include "render_command.h"
#include "resource/shader_library.h"
#include "shader.h"
#include "texture.h"
#include "vertex_array.h"

#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include <array>

namespace Leaper
{

    struct CubeVertex
    {
        glm::vec3 position;
    };

    struct Renderer3DData
    {
        static const uint32_t max_cubes         = 20000;
        static const uint32_t max_vertices      = 36;
        static const uint32_t max_indices       = 36;
        static const uint32_t max_texture_slots = 32;

        uint32_t cube_index_count;
        Ref<VertexArray> cube_vertex_array;
        Ref<VertexBuffer> cube_vertex_buffer;

        CubeVertex* cube_vertex_buffer_base = nullptr;
        CubeVertex* cube_vertex_buffer_ptr  = nullptr;

        Ref<Shader> skybox_shader   = nullptr;
        Ref<Texture> skybox_texture = nullptr;

        std::array<Ref<Texture>, max_texture_slots> texture_slots;
        uint32_t texture_slot_index = 1;  // 0 = white texture
        Ref<Texture> white_texture;

        std::vector<glm::vec3> cube_vertex_position = { glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, -1.0f),  // 后
                                                        glm::vec3(1.0f, 1.0f, -1.0f),   glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, -1.0f),

                                                        glm::vec3(-1.0f, -1.0f, 1.0f),  glm::vec3(1.0f, -1.0f, 1.0f),  glm::vec3(1.0f, 1.0f, 1.0f),  // 前
                                                        glm::vec3(1.0f, 1.0f, 1.0f),    glm::vec3(-1.0f, 1.0f, 1.0f),  glm::vec3(-1.0f, -1.0f, 1.0f),

                                                        glm::vec3(-1.0f, 1.0f, 1.0f),   glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, -1.0f),  // 左
                                                        glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, 1.0f, 1.0f),

                                                        glm::vec3(1.0f, 1.0f, 1.0f),    glm::vec3(1.0f, 1.0f, -1.0f),  glm::vec3(1.0f, -1.0f, -1.0f),  // 右
                                                        glm::vec3(1.0f, -1.0f, -1.0f),  glm::vec3(1.0f, -1.0f, 1.0f),  glm::vec3(1.0f, 1.0f, 1.0f),

                                                        glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, -1.0f, 1.0f),  // 下
                                                        glm::vec3(1.0f, -1.0f, 1.0f),   glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, -1.0f, -1.0f),

                                                        glm::vec3(-1.0f, 1.0f, -1.0f),  glm::vec3(1.0f, 1.0f, -1.0f),  glm::vec3(1.0f, 1.0f, 1.0f),  // 上
                                                        glm::vec3(1.0f, 1.0f, 1.0f),    glm::vec3(-1.0f, 1.0f, 1.0f),  glm::vec3(-1.0f, 1.0f, -1.0f) };

        glm::vec3 cube_normal[36]{ glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f),
                                   glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),
                                   glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 1.0f),
                                   glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f),
                                   glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3(1.0f, 0.0f, 0.0f),
                                   glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3(1.0f, 0.0f, 0.0f),
                                   glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f),
                                   glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f),
                                   glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f) };

        struct CameraBuffer
        {
            glm::mat4 view_ContentBroswerion;
            glm::vec3 position;
        };

        CameraBuffer camera_buffer;
        DirectionalLightComponent directional_light_buffer;
        Ref<UniformBuffer> light_uniform_buffer;

        glm::mat4 view;
        glm::mat4 ContentBroswerion;
    };

    static Renderer3DData s_data;
    Ref<FrameBuffer> Renderer3D::s_frame_buffer = nullptr;

    void Renderer3D::Init()
    {
        s_data.skybox_shader     = ShaderLibrary::GetSkyBoxShader();
        s_data.cube_vertex_array = Leaper::VertexArray::Create();
        s_data.cube_vertex_array->Bind();
        s_data.cube_vertex_buffer = Leaper::VertexBuffer::Create(s_data.cube_vertex_position.data(), sizeof(glm::vec3) * s_data.cube_vertex_position.size());
        s_data.cube_vertex_buffer->SetLayout({ { Leaper::ShaderDataType::Float3, "a_Position" } });

        s_data.cube_vertex_array->AddVertexBuffer(s_data.cube_vertex_buffer);

        s_data.skybox_texture = Texture::CreateCubeMap(std::vector<std::string>({ "./assets/skybox/right.jpg", "./assets/skybox/left.jpg", "./assets/skybox/top.jpg",
                                                                                  "./assets/skybox/bottom.jpg", "./assets/skybox/front.jpg", "./assets/skybox/back.jpg" }));

        s_data.white_texture        = Texture::Create(1, 1);
        uint32_t white_texture_data = 0xffffffff;
        s_data.white_texture->SetData(&white_texture_data);
        s_data.texture_slots[0] = s_data.white_texture;

        s_data.light_uniform_buffer = UniformBuffer::Create(sizeof(DirectionalLightComponent), 1);
        s_frame_buffer              = FrameBuffer::Create();
        s_frame_buffer->CreateDepthMap(2048, 2048);
    }

    void Renderer3D::BeginScene(const glm::mat4& camera, glm::vec3 position)
    {
        s_data.camera_buffer.view_ContentBroswerion = camera;
        s_data.camera_buffer.position               = position;
        StartBatch();
    }
    void Renderer3D::SetCamera(const glm::mat4& view, const glm::mat4& ContentBroswerion, glm::vec3 position)
    {
        s_data.view                   = view;
        s_data.ContentBroswerion      = ContentBroswerion;
        s_data.camera_buffer.position = position;
    }

    void Renderer3D::BeginScene(const glm::mat4& camera, glm::mat4& trans)
    {
        s_data.camera_buffer.view_ContentBroswerion = camera * trans;
        StartBatch();
    }

    void Renderer3D::EndScene()
    {
        // Flush();
    }

    void Renderer3D::StartBatch()
    {
        /*
        s_data.cube_index_count       = 0;
        s_data.cube_vertex_buffer_ptr = s_data.cube_vertex_buffer_base;

        s_data.texture_slot_index = 1;
        */
    }

    void Renderer3D::Flush()
    { /*
      if (s_data.cube_index_count)
      {
          uint32_t data_size = (uint32_t)((uint8_t*)s_data.cube_vertex_buffer_ptr - (uint8_t*)s_data.cube_vertex_buffer_base);
          s_data.cube_vertex_buffer->SetData(s_data.cube_vertex_buffer_base, data_size);

          // bind texture
          for (uint32_t i = 0; i < s_data.texture_slot_index; i++) s_data.texture_slots[i]->Bind(i);

          s_data.cube_shader->Bind();

          s_data.cube_shader->SetMat4("u_ViewContentBroswerion", s_data.camera_buffer.view_ContentBroswerion);
          s_data.cube_shader->SetVec3("u_ViewPosition", s_data.camera_buffer.position);

          s_data.cube_vertex_array->DrawArray(s_data.max_vertices);
      }
      */
    }

    void Renderer3D::NextBatch()
    {
        /*
        Flush();
        StartBatch();
        */
    }

    void Renderer3D::DrawCube(glm::mat4& trans, const Ref<Texture>& texture, glm::vec4 color, int entity_id)
    {
        /*
        constexpr size_t cube_vertex_count   = 36;
        constexpr glm::vec2 texture_coords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

        float texture_index = 0.0f;

        if (s_data.cube_index_count >= Renderer3DData::max_indices)
            NextBatch();

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
        */
    }

    void Renderer3D::DrawSkyBox(std::vector<std::string> faces)
    {
        // RenderCommand::EnableCull();
        // RenderCommand::CullFrontFace();
        glm::mat4 view = glm::mat4(glm::mat3(s_data.view));
        RenderCommand::DepthMask(false);
        s_data.skybox_shader->Bind();
        s_data.skybox_shader->SetMat4("u_View", view);
        s_data.skybox_shader->SetMat4("u_ContentBroswerion", s_data.ContentBroswerion);
        s_data.skybox_texture->BindCubeMap();
        s_data.cube_vertex_array->DrawArray(36);
        RenderCommand::DepthMask(true);
        // RenderCommand::DisableCull();
    }

    void Renderer3D::SetDirectionalLight(glm::vec3 color, float ambient_strength, float specular_strength)
    {
        s_data.directional_light_buffer.color             = color;
        s_data.directional_light_buffer.ambient_strength  = ambient_strength;
        s_data.directional_light_buffer.specular_strength = specular_strength;

        s_data.light_uniform_buffer->SetData(&s_data.directional_light_buffer, sizeof(DirectionalLightComponent));
    }

    void Renderer3D::DrawModel(Model model, glm::mat4& trans, Ref<Shader> shader, int entity_id)
    {
        model.Draw(trans, shader, entity_id);
    }

    void Renderer3D::OnEvent(Event& e) {}

}  // namespace Leaper