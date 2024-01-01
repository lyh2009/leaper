#pragma once
#include "events.h"

namespace Leaper
{
    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(uint32_t width, uint32_t height)
            : m_width(width), m_height(height) {}

        uint32_t GetWidth() const { return m_width; }
        uint32_t GetHeight() const { return m_height; }
        
        EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
    private:
        uint32_t m_width, m_height;
    };

} // namespace Leaper