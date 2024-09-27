#pragma once
#include "../task/render_task_producer.h"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "key_codes.h"

namespace Leaper
{
    class Input
    {
    public:
        /// @brief Determines whether the keyboard key is pressed
        /// @param key_code
        /// @return Press to return true
        static bool IsKeyDown(int key_code)
        {
            return m_input->IsKeyDownImpl(key_code);
        }

        /// @brief Determines whether the keyboard key is release
        /// @param key_code
        /// @return Release to return true
        static bool IsKeyRelease(int key_code)
        {
            return m_input->IsKeyReleaseImpl(key_code);
        }

        /// @brief Determines whether the mouse key is pressed
        /// @param key_code
        /// @return Press to return true
        static bool IsMouseDown(int key_code)
        {
            return m_input->IsMouseDownImpl(key_code);
        }

        static glm::vec2 GetMousePostion()
        {
            return RenderTaskProducer::ProduceRenderTaskInput_GetMousePos();
        }

    protected:
        virtual bool IsKeyDownImpl(int key_code)    = 0;
        virtual bool IsKeyReleaseImpl(int key_code) = 0;
        virtual bool IsMouseDownImpl(int key_code)  = 0;
        virtual glm::vec2 GetMousePositionImpl()    = 0;

    private:
        static Leaper::Input* m_input;
    };

}  // namespace Leaper
