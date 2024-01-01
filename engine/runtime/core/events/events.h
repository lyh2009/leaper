#pragma once

#include "core/base.h"

#include <string>
#include <functional>

namespace Leaper
{
    enum class EventType
    {
        None = 0, WindowClose, WindowResize, WindowMoved,
        KeyPressed, KeyReleased,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    enum EventCategory
	{
		None = 0,
		EventCategoryApplication    = BIT(0),
		EventCategoryInput          = BIT(1),
		EventCategoryKeyboard       = BIT(2),
		EventCategoryMouse          = BIT(3),
		EventCategoryMouseButton    = BIT(4)
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
                               virtual EventType GetEventType() const override { return GetStaticType(); }\
                               virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

    class Event
    {
    public:
        bool handle = true;

        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual std::string ToString() const { return GetName(); }

        bool IsInCategory(EventCategory category)
        {
            return GetCategoryFlags() & category;
        }
    };

    class EventDispatcher
    {
    public:
        EventDispatcher(Event& event)
            : m_event(event) {}
        
        template<typename T, typename F>
        bool Dispatch(const F& func)
        {
            if(m_event.GetEventType() == T::GetStaticType())
            {
                m_event.handle |= func(static_cast<T&>(m_event));
                return true;
            }
            return false;
        }
    
    private:
        Event& m_event;
    };
    
} // namespace Leaper
