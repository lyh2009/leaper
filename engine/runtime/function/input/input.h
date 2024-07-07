#pragma once
#include "key_codes.h"

namespace Leaper
{
    class Input
    {
    public:
        /// @brief Determines whether the keyboard key is pressed
        /// @param key_code
        /// @return Press to return true
        static bool IsKeyDown(int key_code) { return m_input->IsKeyDownImpl(key_code); }

        /// @brief Determines whether the keyboard key is release
        /// @param key_code
        /// @return Release to return true
        static bool IsKeyRelease(int key_code) { return m_input->IsKeyReleaseImpl(key_code); }

        /// @brief Determines whether the mouse key is pressed
        /// @param key_code
        /// @return Press to return true
        static bool IsMouseDown(int key_code) { return m_input->IsMouseDownImpl(key_code); }

    protected:
        virtual bool IsKeyDownImpl(int key_code) = 0;
        virtual bool IsKeyReleaseImpl(int key_code) = 0;
        virtual bool IsMouseDownImpl(int key_code) = 0;

    private:
        static Leaper::Input *m_input;
    };

} // namespace Leaper
