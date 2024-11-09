#pragma once
#include "function/input/input.h"

namespace Leaper
{

    class OpenGLInput : public Leaper::Input
    {
    protected:
        virtual bool IsKeyDownImpl(int key_code) override;
        virtual bool IsKeyReleaseImpl(int key_code) override;
        virtual bool IsMouseDownImpl(int key_code) override;
        virtual glm::vec2 GetMousePositionImpl() override;
    };

}  // namespace Leaper