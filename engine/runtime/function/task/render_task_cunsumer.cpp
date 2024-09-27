#include "function/input/input.h"
#include "function/render/buffers.h"
#include "function/render/framebuffer.h"
#include "function/render/render_command.h"
#include "function/render/vertex_array.h"
#include "render_task_cunsumer.h"
#include "render_task_types.h"
#include <mutex>
#include <thread>

namespace Leaper
{
    std::mutex RenderTaskCunsumer::m_mut;
    void RenderTaskCunsumer::Shader_Create(RenderTaskBase* task)
    {

        auto shader_task               = dynamic_cast<RenderTaskShader_Create*>(task);
        shader_task->shader            = Shader::Create(shader_task->vertex_shader_source, shader_task->fragment_shader_source);
        shader_task->return_result_set = true;
    }
    // -----------------------------------------------VertexArray-------------------------------------------------
    void RenderTaskCunsumer::CreateVAO(RenderTaskBase* task)
    {
        auto vao_task               = dynamic_cast<RenderTaskVAO_Create*>(task);
        vao_task->vao               = VertexArray::Create();
        vao_task->return_result_set = true;
    }
    void RenderTaskCunsumer::BindVAO(RenderTaskBase* task)
    {
        auto vao_task = dynamic_cast<RenderTaskVAO_Bind*>(task);
        vao_task->vao->Bind();
    }
    void RenderTaskCunsumer::UnBindVAO(RenderTaskBase* task)
    {
        auto vao_task = dynamic_cast<RenderTaskVAO_UnBind*>(task);
        vao_task->vao->UnBind();
    }
    void RenderTaskCunsumer::VAO_SetVBO(RenderTaskBase* task)
    {
        auto vao_task = dynamic_cast<RenderTaskVAO_SetVBO*>(task);
        vao_task->vao->AddVertexBuffer(vao_task->vbo);
    }
    void RenderTaskCunsumer::VAO_SetIBO(RenderTaskBase* task)
    {
        auto vao_task = dynamic_cast<RenderTaskVAO_SetIBO*>(task);
        vao_task->vao->SetIndexBuffer(vao_task->ibo);
    }

    // ----------------------------------------Vertex Buffer------------------------------------
    void RenderTaskCunsumer::CreateVBO(RenderTaskBase* task)
    {
        auto vbo_task               = dynamic_cast<RenderTaskVBO_Create*>(task);
        vbo_task->vbo               = VertexBuffer::Create(vbo_task->vertices, vbo_task->size);
        vbo_task->return_result_set = true;
    }
    void RenderTaskCunsumer::BindVBO(RenderTaskBase* task)
    {
        auto vbo_task = dynamic_cast<RenderTaskVBO_Bind*>(task);
        vbo_task->vbo->Bind();
    }
    void RenderTaskCunsumer::UnBindVBO(RenderTaskBase* task)
    {
        auto vbo_task = dynamic_cast<RenderTaskVBO_UnBind*>(task);
        vbo_task->vbo->UnBind();
    }
    void RenderTaskCunsumer::VBO_SetData(RenderTaskBase* task)
    {
        auto vbo_task = dynamic_cast<RenderTaskVBO_SetData*>(task);
        vbo_task->vbo->SetData(vbo_task->vertices, vbo_task->size);
    }

    // ----------------------------------------Fragment Buffer-----------------------------------------------
    void RenderTaskCunsumer::CreateFBO(RenderTaskBase* task)
    {
        auto fbo               = dynamic_cast<RenderTaskFBO_Create*>(task);
        fbo->fbo               = FrameBuffer::Create();
        fbo->return_result_set = true;
    }
    void RenderTaskCunsumer::BindFBO(RenderTaskBase* task)
    {
        auto fbo = dynamic_cast<RenderTaskFBO_Bind*>(task);
        fbo->fbo->Bind();
    }
    void RenderTaskCunsumer::UnBindFBO(RenderTaskBase* task)
    {
        auto fbo = dynamic_cast<RenderTaskFBO_UnBind*>(task);
        fbo->fbo->Unbind();
    }
    void RenderTaskCunsumer::ClearAttachment(RenderTaskBase* task)
    {
        auto fbo = dynamic_cast<RenderTaskFBO_ClearAttachment*>(task);
        fbo->fbo->ClearAttachment(fbo->attachment_index, fbo->value);
    }
    // --------------------------------------------Input--------------------------------------------
    void RenderTaskCunsumer::GetMousePos(RenderTaskBase* task)
    {
        auto input              = dynamic_cast<RenderTaskInput_GetMousePos*>(task);
        input->pos              = Input::GetMousePostion();
        task->return_result_set = true;
    }

    // --------------------------------------------Render Command--------------------------------------
    void RenderTaskCunsumer::ClearColor(RenderTaskBase* task)
    {
        auto clear_task = dynamic_cast<RenderTask_ClearColor*>(task);
        RenderCommand::SetClearColor(clear_task->color);
    }
    void RenderTaskCunsumer::DrawElements(RenderTaskBase* task)
    {
        auto draw_task = dynamic_cast<RenderTask_DrawElement*>(task);
        RenderCommand::DrawElements(draw_task->vao, draw_task->count);
    }

    void RenderTaskCunsumer::Model_Create(RenderTaskBase* task)
    {
        auto model_task               = dynamic_cast<RenderTaskModel_Create*>(task);
        model_task->model             = Model(model_task->path);
        model_task->return_result_set = true;
    }
}  // namespace Leaper