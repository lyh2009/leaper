#include "core/base.h"
#include "function/render/buffers.h"
#include "function/task/render_task_queue.h"
#include "render_task_producer.h"
#include "render_task_types.h"

namespace Leaper
{
    void RenderTaskProducer::ProduceRenderTaskShader_Create(std::string vertex_source, std::string fragment_source, Ref<Shader> shader)
    {
        Ref<RenderTaskShader_Create> task = CreateRef<RenderTaskShader_Create>();
        task->fragment_shader_source      = fragment_source;
        task->vertex_shader_source        = vertex_source;
        task->need_return_result          = true;
        RenderTaskQueue::Push(task.get());

        // 等待编译完成
        // task->Wait();
        shader = task->shader;
    }

    // -----------------------------------------------Vertex Array------------------------------------------------
    Ref<VertexArray> RenderTaskProducer::ProduceRenderTaskVAO_Create()
    {

        Ref<RenderTaskVAO_Create> task = CreateRef<RenderTaskVAO_Create>();
        RenderTaskQueue::Push(task.get());

        task->is_produce = true;
        return task->vao;
    }

    void RenderTaskProducer::ProduceRenderTaskVAO_Bind(VertexArray* vao)
    {
        RenderTaskVAO_Bind* task = new RenderTaskVAO_Bind();
        task->vao                = vao;
        RenderTaskQueue::Push(task);
    }
    void RenderTaskProducer::ProduceRenderTaskVAO_UnBind(VertexArray* vao)
    {
        RenderTaskVAO_UnBind* task = new RenderTaskVAO_UnBind();
        task->vao                  = vao;
        RenderTaskQueue::Push(task);
    }
    void RenderTaskProducer::ProduceRenderTaskVAO_SetVBO(VertexArray* vao, Ref<VertexBuffer> vbo)
    {
        RenderTaskVAO_SetVBO* task = new RenderTaskVAO_SetVBO();
        task->vao                  = vao;
        task->vbo                  = vbo;

        RenderTaskQueue::Push(task);
    }
    void RenderTaskProducer::ProduceRenderTaskVAO_SetIBO(VertexArray* vao, Ref<IndexBuffer> ibo)
    {
        RenderTaskVAO_SetIBO* task = new RenderTaskVAO_SetIBO();
        task->vao                  = vao;
        task->ibo                  = ibo;

        RenderTaskQueue::Push(task);
    }
    // ---------------------------------------Vertex Buffer------------------------------------------
    Ref<VertexBuffer> RenderTaskProducer::ProduceRenderTaskVBO_Create(const void* vertices, size_t size)
    {
        Ref<RenderTaskVBO_Create> task = CreateRef<RenderTaskVBO_Create>();
        task->size                     = size;
        task->vertices                 = vertices;
        RenderTaskQueue::Push(task.get());
        task->is_produce = true;

        return task->vbo;
    }
    void RenderTaskProducer::ProduceRenderTaskVBO_Bind(VertexBuffer* vbo)
    {
        RenderTaskVBO_Bind* task = new RenderTaskVBO_Bind();
        task->vbo                = vbo;
        RenderTaskQueue::Push(task);
    }
    void RenderTaskProducer::ProduceRenderTaskVBO_UnBind(VertexBuffer* vbo)
    {
        RenderTaskVBO_UnBind* task = new RenderTaskVBO_UnBind();
        task->vbo                  = vbo;
        RenderTaskQueue::Push(task);
    }

    void RenderTaskProducer::ProduceRenderTaskVBO_SetData(VertexBuffer* vbo, const void* vertices, size_t size)
    {
        RenderTaskVBO_SetData* task = new RenderTaskVBO_SetData();
        task->vbo                   = vbo;
        task->vertices              = vertices;
        task->size                  = size;
        RenderTaskQueue::Push(task);
    }

    // ----------------------------------Frame Buffer-------------------------------------
    Ref<FrameBuffer> RenderTaskProducer::ProduceRenderTaskFBO_Create()
    {
        Ref<RenderTaskFBO_Create> task = CreateRef<RenderTaskFBO_Create>();
        RenderTaskQueue::Push(task.get());

        return task->fbo;
    }
    void RenderTaskProducer::ProduceRenderTaskFBO_Bind(FrameBuffer* fbo)
    {
        RenderTaskFBO_Bind* task = new RenderTaskFBO_Bind();
        task->fbo                = fbo;
        RenderTaskQueue::Push(task);
    }
    void RenderTaskProducer::ProduceRenderTaskFBO_UnBind(FrameBuffer* fbo)
    {
        RenderTaskFBO_UnBind* task = new RenderTaskFBO_UnBind();
        task->fbo                  = fbo;
        RenderTaskQueue::Push(task);
    }
    void RenderTaskProducer::ProduceRenderTaskFBO_ClearAttachment(FrameBuffer* fbo, uint32_t attachment_index, int value)
    {
        auto task              = new RenderTaskFBO_ClearAttachment();
        task->fbo              = fbo;
        task->attachment_index = attachment_index;
        task->value            = value;
        RenderTaskQueue::Push(task);
    }
    // -------------------------------------------Input------------------------------------------
    glm::vec2 RenderTaskProducer::ProduceRenderTaskInput_GetMousePos()
    {
        auto task = CreateRef<RenderTaskInput_GetMousePos>();
        RenderTaskQueue::Push(task.get());

        return task->pos;
    }

    // ----------------------------------Render Command--------------------------------------
    void RenderTaskProducer::ProduceRenderTask_DrawElement(Ref<VertexArray>& vao, uint32_t count)
    {
        RenderTask_DrawElement* task = new RenderTask_DrawElement();
        task->count                  = count;
        task->vao                    = vao;
        RenderTaskQueue::Push(task);
    }

    void RenderTaskProducer::ProduceRenderTask_EndFrame()
    {
        RenderTask_EndFrame* render_task_frame_end = new RenderTask_EndFrame();
        RenderTaskQueue::Push(render_task_frame_end);
        // 等待渲染结束任务，说明渲染线程渲染完了这一帧所有的东西。
        render_task_frame_end->Wait();
        delete render_task_frame_end;  // 需要等待结果的任务，需要在获取结果后删除。
    }

    void RenderTaskProducer::ProduceRenderTask_ClearColor(glm::vec4 color)
    {
        RenderTask_ClearColor* task = new RenderTask_ClearColor();
        task->color                 = color;
        RenderTaskQueue::Push(task);
    }

    Model RenderTaskProducer::ProduceRenderTaskModel_Create(const std::string& path)
    {
        auto task  = new RenderTaskModel_Create();
        task->path = path;
        RenderTaskQueue::Push(task);
        task->Wait();
        return task->model;
    }
}  // namespace Leaper