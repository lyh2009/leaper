#pragma once
#include "../render/shader.h"
#include "core/base.h"
#include "function/render/3d/model.h"
#include "function/render/buffers.h"
#include "function/render/framebuffer.h"
#include "function/render/vertex_array.h"
#include <string>

namespace Leaper
{
    class RenderTaskProducer
    {
    public:
        static void ProduceRenderTaskShader_Create(std::string vertex_source, std::string fragment_source, Ref<Shader> shader);
        // Vertex Array
        static Ref<VertexArray> ProduceRenderTaskVAO_Create();
        static void ProduceRenderTaskVAO_Bind(VertexArray* vao);
        static void ProduceRenderTaskVAO_UnBind(VertexArray* vao);
        static void ProduceRenderTaskVAO_SetVBO(VertexArray* vao, Ref<VertexBuffer> vbo);
        static void ProduceRenderTaskVAO_SetIBO(VertexArray* vao, Ref<IndexBuffer> ibo);
        // Vertex Buffer
        static Ref<VertexBuffer> ProduceRenderTaskVBO_Create(const void* vertices, size_t size);
        static void ProduceRenderTaskVBO_Bind(VertexBuffer* vbo);
        static void ProduceRenderTaskVBO_UnBind(VertexBuffer* vbo);
        static void ProduceRenderTaskVBO_SetData(VertexBuffer* vbo, const void* vertices, size_t size);
        // Frame Buffer
        static Ref<FrameBuffer> ProduceRenderTaskFBO_Create();
        static void ProduceRenderTaskFBO_Bind(FrameBuffer* fbo);
        static void ProduceRenderTaskFBO_UnBind(FrameBuffer* fbo);
        static void ProduceRenderTaskFBO_ClearAttachment(FrameBuffer* fbo, uint32_t attachment_index, int value);
        // Input
        static glm::vec2 ProduceRenderTaskInput_GetMousePos();
        // Render Command
        static void ProduceRenderTask_DrawElement(Ref<VertexArray>& vao, uint32_t count);
        static void ProduceRenderTask_ClearColor(glm::vec4 color);
        static void ProduceRenderTask_EndFrame();

        // model
        static Model ProduceRenderTaskModel_Create(const std::string& path);
    };
}  // namespace Leaper
