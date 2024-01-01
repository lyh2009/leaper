#pragma once
#include "events.h"
#include "function/input/mouse_codes.h"

#include <sstream>
namespace Leaper
{
    class MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(const float x, const float y) 
            : m_mouse_x(x), m_mouse_y(y) {}

        float GetMouseX() const { return m_mouse_x; }
        float GetMouseY() const { return m_mouse_y; }

        EVENT_CLASS_TYPE(MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        float m_mouse_x, m_mouse_y;
    };
    
    class MouseScrolledEvent : public Event
    {
    public:
        MouseScrolledEvent(float xoffset, float yoffset)
            : m_xoffset(xoffset), m_yoffset(yoffset) {}
        
        float GetXOffset() const { return m_xoffset; }
        float GetYOffset() const { return m_yoffset; }

        EVENT_CLASS_TYPE(MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategoryMouse)
    private:
        float m_xoffset, m_yoffset; 
    };

    class MouseButtonEvent : public Event
    {
    public:
        Leaper::MouseButton GetMouseButton() const { return m_button; }

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
    protected:
        MouseButtonEvent(Leaper::MouseButton button)
            : m_button(button) {}
        Leaper::MouseButton m_button;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(const MouseButton button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(const MouseButton button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

} // namespace Leaper
