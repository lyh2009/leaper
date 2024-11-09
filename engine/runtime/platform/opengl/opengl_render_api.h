#pragma once
#include "core/base.h"
#include "function/render/render_api.h"

#include <glad/glad.h>

namespace Leaper
{

    class OpenGLRenderAPI : public RenderAPI
    {
    public:
        OpenGLRenderAPI();
        ~OpenGLRenderAPI();
        virtual void Init() override;
        virtual void SetClearColor(glm::vec4 color) override;
        virtual void Clear() override;
        virtual void DrawElements(Ref<VertexArray> vertex_array, uint32_t count) override;
        virtual void DrawLines(const Ref<VertexArray>& vao, uint32_t count) override;
        virtual void SetLineWidth(const float& width) override;
        virtual void CullFrontFace() const override;
        virtual void DisableCull() const override;
        virtual void EnableCull() const override;
        virtual void DepthMask(bool enable) override;
    };
}  // namespace Leaper