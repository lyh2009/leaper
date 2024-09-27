#pragma once
#include "../render/render_command.h"
#include "../render/shader.h"
#include "../render/vertex_array.h"
#include "core/base.h"
#include "core/log.h"
#include "function/render/3d/model.h"
#include "function/render/buffers.h"
#include "function/render/framebuffer.h"
#include "glm/fwd.hpp"
#include "magic_enum.hpp"
#include "task_commands.h"
#include <cstddef>
#include <cstdint>
#include <thread>


namespace Leaper
{
    class RenderTaskBase
    {
    public:
        RenderTaskBase(bool need_return_result = false, bool return_result_set = false)
        {
            this->need_return_result = need_return_result;
            this->return_result_set  = return_result_set;
        }
        virtual ~RenderTaskBase() {};

    public:
        RenderTaskCommand commands = NONE;
        bool need_return_result    = false;
        bool return_result_set     = false;
        bool is_produce            = false;
    };

    class RenderTaskNeedReturnResult : public RenderTaskBase
    {
    public:
        RenderTaskNeedReturnResult()
        {
            commands           = RenderTaskCommand::NONE;
            need_return_result = true;
        }
        ~RenderTaskNeedReturnResult() {}

        /// 等待任务在渲染线程执行完毕，并设置回传结果。主线程拿到结果后才能执行下一步代码。
        virtual void Wait()
        {
            while (return_result_set == false) { std::this_thread::sleep_for(std::chrono::microseconds(1)); }
        }
    };

    class RenderTaskShader_Create : public RenderTaskNeedReturnResult
    {
    public:
        RenderTaskShader_Create()
        {
            commands = RenderTaskCommand::SHADER_CREATE;
        }
        ~RenderTaskShader_Create() {}

    public:
        std::string vertex_shader_source   = nullptr;
        std::string fragment_shader_source = nullptr;

    public:
        Ref<Shader> shader = nullptr;
    };

    // ------------------------------Create VAO-----------------------------------
    class RenderTaskVAO_Create : public RenderTaskNeedReturnResult
    {
    public:
        RenderTaskVAO_Create()
        {
            commands = RenderTaskCommand::VAO_CREATE;
        }
        ~RenderTaskVAO_Create() {}

    public:
        Ref<VertexArray> vao = nullptr;
    };

    // VertexArray::Bind()
    class RenderTaskVAO_Bind : public RenderTaskBase
    {
    public:
        RenderTaskVAO_Bind()
        {
            commands = RenderTaskCommand::VAO_BIND;
        }
        ~RenderTaskVAO_Bind() {}

    public:
        VertexArray* vao = nullptr;
    };

    // VertexArray::UnBind()
    class RenderTaskVAO_UnBind : public RenderTaskBase
    {
    public:
        RenderTaskVAO_UnBind()
        {
            commands = RenderTaskCommand::VAO_UNBIND;
        }
        ~RenderTaskVAO_UnBind() {}

    public:
        VertexArray* vao = nullptr;
    };

    // VertexArray::SetVertexBuffer(Leaper::Ref<Leaper::VertexBuffer> vertex_buffer)
    class RenderTaskVAO_SetVBO : public RenderTaskBase
    {
    public:
        RenderTaskVAO_SetVBO()
        {
            commands = RenderTaskCommand::VAO_SET_VBO;
        }
        ~RenderTaskVAO_SetVBO() {}

    public:
        VertexArray* vao      = nullptr;
        Ref<VertexBuffer> vbo = nullptr;
    };
    // VertexArray::SetIndexBuffer(Leaper::Ref<Leaper::IndexBuffer> buffer)
    class RenderTaskVAO_SetIBO : public RenderTaskBase
    {
    public:
        RenderTaskVAO_SetIBO()
        {
            commands = RenderTaskCommand::VAO_SET_IBO;
        }
        ~RenderTaskVAO_SetIBO() {}

    public:
        VertexArray* vao     = nullptr;
        Ref<IndexBuffer> ibo = nullptr;
    };

    // -----------------------------Create VertexBuffer----------------------------------
    class RenderTaskVBO_Create : public RenderTaskNeedReturnResult
    {
    public:
        RenderTaskVBO_Create()
        {
            commands = RenderTaskCommand::VBO_CREATE;
        }
        ~RenderTaskVBO_Create() {}

    public:
        const void* vertices;
        size_t size;
        Ref<VertexBuffer> vbo = nullptr;
    };
    // VertexBuffer::Bind()
    class RenderTaskVBO_Bind : public RenderTaskBase
    {
    public:
        RenderTaskVBO_Bind()
        {
            commands = RenderTaskCommand::VBO_BIND;
        }
        ~RenderTaskVBO_Bind() {}

    public:
        VertexBuffer* vbo = nullptr;
    };
    // VertexBuffer::UnBind()
    class RenderTaskVBO_UnBind : public RenderTaskBase
    {
    public:
        RenderTaskVBO_UnBind()
        {
            commands = RenderTaskCommand::VBO_UNBIND;
        }
        ~RenderTaskVBO_UnBind() {}

    public:
        VertexBuffer* vbo = nullptr;
    };

    // VertexBuffer::SetData(const void* vertices, size_t size)
    class RenderTaskVBO_SetData : public RenderTaskBase
    {
    public:
        RenderTaskVBO_SetData()
        {
            commands = RenderTaskCommand::VBO_SET_DATA;
        }
        ~RenderTaskVBO_SetData() {}

    public:
        const void* vertices;
        size_t size;
        VertexBuffer* vbo = nullptr;
    };

    // -----------------------Create FrameBuffer------------------------------------
    class RenderTaskFBO_Create : public RenderTaskNeedReturnResult
    {
    public:
        RenderTaskFBO_Create()
        {
            commands = RenderTaskCommand::FBO_CREATE;
        }
        ~RenderTaskFBO_Create() {}

    public:
        Ref<FrameBuffer> fbo = nullptr;
    };
    // FrameBuffer::Bind()
    class RenderTaskFBO_Bind : public RenderTaskBase
    {
    public:
        RenderTaskFBO_Bind()
        {
            commands = RenderTaskCommand::FBO_BIND;
        }
        ~RenderTaskFBO_Bind() {}

    public:
        FrameBuffer* fbo = nullptr;
    };
    // FrameBuffer::UnBind()
    class RenderTaskFBO_UnBind : public RenderTaskBase
    {
    public:
        RenderTaskFBO_UnBind()
        {
            commands = RenderTaskCommand::FBO_UNBIND;
        }
        ~RenderTaskFBO_UnBind() {}

    public:
        FrameBuffer* fbo = nullptr;
    };

    // FrameBuffer::CreateTexture
    class RenderTaskFBO_CreateTexture : public RenderTaskNeedReturnResult
    {
    public:
        RenderTaskFBO_CreateTexture()
        {
            commands = RenderTaskCommand::FBO_CREATE_TEXTURE;
        }
        ~RenderTaskFBO_CreateTexture() {}

    public:
        FrameBuffer* fbo = nullptr;
    };

    // FrameBuffer::CreateTexture
    class RenderTaskFBO_ClearAttachment : public RenderTaskBase
    {
    public:
        RenderTaskFBO_ClearAttachment()
        {
            commands = RenderTaskCommand::FBO_CLEAR_ATTACHMENT;
        }
        ~RenderTaskFBO_ClearAttachment() {}

    public:
        FrameBuffer* fbo = nullptr;
        uint32_t attachment_index;
        int value;
    };
    // ------------------------------------------------Input---------------------------------------------
    class RenderTaskInput_GetMousePos : public RenderTaskNeedReturnResult
    {
    public:
        RenderTaskInput_GetMousePos()
        {
            commands = RenderTaskCommand::INPUT_GET_MOUSE_POS;
        }
        ~RenderTaskInput_GetMousePos() {}

    public:
        glm::vec2 pos;
    };

    // -------------------------------Render Command-----------------------------------------------------------
    // RenderCommand::DrawElement(Ref<VertexArray> vao, uint32_t count)
    class RenderTask_DrawElement : public RenderTaskBase
    {
    public:
        RenderTask_DrawElement()
        {
            commands = RenderTaskCommand::DRAW_ELEMENT;
        }
        ~RenderTask_DrawElement() {}

    public:
        uint32_t count       = 0;
        Ref<VertexArray> vao = nullptr;
    };

    // RenderCommand::ClearColor(glm::vec4 color)
    class RenderTask_ClearColor : public RenderTaskBase
    {
    public:
        RenderTask_ClearColor()
        {
            commands = RenderTaskCommand::CLEAR_COLOR;
        }
        ~RenderTask_ClearColor() {};

    public:
        glm::vec4 color;
    };

    /// 特殊任务：帧结束标志，渲染线程收到这个任务后，刷新缓冲区，设置帧结束。
    class RenderTask_EndFrame : public RenderTaskNeedReturnResult
    {
    public:
        RenderTask_EndFrame()
        {
            commands = RenderTaskCommand::END_FRAME;
        }
        ~RenderTask_EndFrame() {}
    };

    class RenderTaskModel_Create : public RenderTaskNeedReturnResult
    {
    public:
        RenderTaskModel_Create()
        {
            commands = RenderTaskCommand::MODEL_CREATE;
        }
        ~RenderTaskModel_Create() {}

    public:
        std::string path;
        Model model;
    };

}  // namespace Leaper