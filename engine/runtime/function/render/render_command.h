#pragma once
#include "function/render/framebuffer.h"
#include "glm/fwd.hpp"
#include "vertex_array.h"
#include <glm/glm.hpp>

namespace Leaper
{
    class RenderCommand
    {
    public:
        static void SetClearColor(glm::vec4 color);
        static void DrawElements(Leaper::Ref<Leaper::VertexArray> vertex_array, uint32_t count);
        static void DrawLines(const Leaper::Ref<Leaper::VertexArray>& vao, uint32_t count);
        static void SetLineWidth(float width);
        static void Clear();

        static void CullFrontFace();
        static void EnableCull();
        static void DisableCull();

        static void DepthMask(bool enable);

        static void SetMainFrameBuffer(Ref<FrameBuffer> fbo)
        {
            s_main_frame_buffer = fbo;
        }

        static Ref<FrameBuffer> GetMainFrameBuffer()
        {
            return s_main_frame_buffer;
        }

        static void SetLightSpaceMat(glm::mat4 mat)
        {
            s_light_space_mat = mat;
        }

        static glm::mat4& GetLigthSpaceMat()
        {
            return s_light_space_mat;
        }

    private:
        static glm::mat4 s_light_space_mat;
        static Ref<FrameBuffer> s_main_frame_buffer;
    };
}  // namespace Leaper