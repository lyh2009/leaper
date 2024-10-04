#pragma once
#include "buffers.h"
#include "vertex_array.h"

#include <glm/glm.hpp>

#include <stdint.h>

namespace Leaper
{
    class RenderAPI
    {
    public:
        enum class API
        {
            None,
            OpenGL
        };

    public:
        virtual void Init()                                                                      = 0;
        virtual void SetClearColor(glm::vec4 color)                                              = 0;
        virtual void Clear()                                                                     = 0;
        virtual void DrawElements(Leaper::Ref<Leaper::VertexArray> vertex_array, uint32_t count) = 0;
        virtual void DrawLines(const Leaper::Ref<Leaper::VertexArray>& vao, uint32_t count)      = 0;
        virtual void SetLineWidth(const float& width)                                            = 0;
        virtual void CullFrontFace() const                                                       = 0;
        virtual void DisableCull() const                                                         = 0;
        virtual void EnableCull() const                                                          = 0;
        virtual void DepthMask(bool enable)                                                      = 0;

        static API GetAPI()
        {
            return m_static_api;
        }

        static Leaper::Ref<RenderAPI> Create(API api);

    private:
        static API m_static_api;
    };
}  // namespace Leaper
