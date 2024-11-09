#pragma once
#include "core/log.h"
#include <imgui.h>

namespace Leaper
{
    class Console
    {
    public:
        void OnAttach();
        void OnImGuiRender();

    private:
        bool m_scroll_to_lastest;
        bool m_enable_scroll_to_latest;
        bool m_previous_scroll_y;
    };
}  // namespace Leaper