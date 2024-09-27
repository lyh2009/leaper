#pragma once
#include "render_task_types.h"
#include <functional>
#include <mutex>
#include <thread>

namespace Leaper
{
    class RenderTaskCunsumer
    {
    public:
        static void Shader_Create(RenderTaskBase* task);

        static void CreateVAO(RenderTaskBase* task);
        static void BindVAO(RenderTaskBase* task);
        static void UnBindVAO(RenderTaskBase* task);
        static void VAO_SetVBO(RenderTaskBase* task);
        static void VAO_SetIBO(RenderTaskBase* task);

        static void CreateVBO(RenderTaskBase* task);
        static void BindVBO(RenderTaskBase* task);
        static void UnBindVBO(RenderTaskBase* task);
        static void VBO_SetData(RenderTaskBase* task);

        static void CreateFBO(RenderTaskBase* task);
        static void BindFBO(RenderTaskBase* task);
        static void UnBindFBO(RenderTaskBase* task);
        static void ClearAttachment(RenderTaskBase* task);

        static void GetMousePos(RenderTaskBase* task);

        static void ClearColor(RenderTaskBase* task);
        static void DrawElements(RenderTaskBase* task);

        static void Model_Create(RenderTaskBase* task);

        static std::mutex& GetMutex()
        {
            return m_mut;
        }

    private:
        static std::mutex m_mut;
    };
}  // namespace Leaper