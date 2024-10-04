#include "function/render/render_command.h"
#include "function/render/uniform_buffer.h"
#include "lppch.h"
#include "render_system.h"

#include "function/ecs/components.h"
#include "function/ecs/scene.h"
#include "function/render/3d/mesh.h"
#include "function/render/3d/model.h"
#include "function/render/renderer2d.h"
#include "function/render/renderer3d.h"
#include "resource/shader_library.h"

namespace Leaper
{
    void RenderSystem::OnAttach()
    {
        shadow_shader = Shader::CreateNative("./resource/shaders/depth_map.glsl");
    }
    void RenderSystem::OnUpdate()
    {

        // sprite
        m_scene->Reg().view<SpriteRendererComponent>().each([=](auto other, SpriteRendererComponent& src) {
            Entity entity = { other, m_scene };
            auto& trans   = entity.GetComponent<TransformComponent>();

            if (entity.HasComponent<Animation2DComponent>())
                src.anim = entity.GetComponent<Animation2DComponent>();
            Renderer2D::DrawSprite(trans, src.anim, src.m_texture, src.color, (int)other);
        });

        // light
        {
            int i     = 0;
            auto view = m_scene->Reg().view<TransformComponent, LightComponent>();
            for (auto entity : view)
            {
                auto [trans, light] = view.get<TransformComponent, LightComponent>(entity);
                light.id            = i;
                Renderer2D::DrawLight(trans, light);
                i++;
            }
        }
        // depth pass
        shadow_shader         = ShaderLibrary::GetDepthMapShader();
        auto main_framebuffer = RenderCommand::GetMainFrameBuffer();
        auto depth_map        = Renderer3D::s_frame_buffer;

        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 0.1f, far_plane = 75.0f;
        lightProjection  = glm::ortho(-35.0f, 35.0f, -35.0f, 35.0f, near_plane, far_plane);
        lightView        = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        shadow_shader->Bind();
        shadow_shader->SetMat4("lightSpaceMat", lightSpaceMatrix);

        depth_map->BindDepthFBO();
        m_scene->Reg().view<MeshRendererComponment>().each([=](auto other, MeshRendererComponment& mrc) {
            Entity entity = { other, m_scene };
            auto& trans   = entity.GetComponent<TransformComponent>();
            Renderer3D::DrawModel(mrc.model, trans.GetTransform(), shadow_shader, (int)other);
        });

        depth_map->UnBindDepthFBO();

        // DirectionalLight
        main_framebuffer->Bind();
        main_framebuffer->ClearAttachment(1, -1);
        auto shader = ShaderLibrary::GetDeffaultShader();
        shader->Bind();

        m_scene->Reg().view<DirectionalLightComponent>().each([=](auto other, DirectionalLightComponent& dlc) {
            Entity entity = { other, m_scene };
            shader->SetMat4("lightSpaceMat", lightSpaceMatrix);
            shader->SetVec3("lightPos", entity.GetComponent<TransformComponent>().position);
            lightPos = entity.GetComponent<TransformComponent>().position;
            Renderer3D::SetDirectionalLight(dlc.color, dlc.ambient_strength, dlc.specular_strength);
            Renderer3D::DrawSkyBox(std::vector<std::string>({ "skybox" }));
        });

        // render pass
        m_scene->Reg().view<MeshRendererComponment>().each([=](auto other, MeshRendererComponment& mrc) {
            Entity entity = { other, m_scene };
            auto& trans   = entity.GetComponent<TransformComponent>();
            shader->Bind();
            depth_map->BindDepthTexture(0);
            Renderer3D::DrawModel(mrc.model, trans.GetTransform(), shader, (int)other);
        });
    }

}  // namespace Leaper