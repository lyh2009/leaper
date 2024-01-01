#pragma once
#include "events.h"
#include "function/input/key_codes.h"

namespace Leaper
{
    class KeyEvent : public Event
    {
    public:
        Leaper::KeyCode GetKeyCode() const { return m_keycode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
    protected:
        KeyEvent(Leaper::KeyCode keycode)
            : m_keycode(keycode) {}

        Leaper::KeyCode m_keycode;
    };

    class KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(const Leaper::KeyCode keycode, bool is_repeat = false)
            : KeyEvent(keycode), m_is_repeat(is_repeat) {}

        bool IsRepeat() const { return m_is_repeat; }

        EVENT_CLASS_TYPE(KeyPressed)
        EVENT_CLASS_CATEGORY(EventCategoryKeyboard)
    private:
        bool m_is_repeat;
    };
} // namespace Leaper
