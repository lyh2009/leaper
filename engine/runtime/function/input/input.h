#pragma once
#include "key_codes.h"

namespace Leaper
{
    class Input
    {
    public:
        /// @brief Determine if a key is pressed
        /// @param key_code 
        static bool IsKeyDown(int key_code) { return m_input->IsKeyDownImpl(key_code); }
        static bool IsKeyRelease(int key_code) { return m_input->IsKeyReleaseImpl(key_code); }
        static bool IsMouseDown(int key_code) { return m_input->IsMouseDownImpl(key_code); }
    protected:
        virtual bool IsKeyDownImpl(int key_code) = 0;
        virtual bool IsKeyReleaseImpl(int key_code) = 0;
        virtual bool IsMouseDownImpl(int key_code) = 0;
    private:
        static Leaper::Input *m_input;
    };

} // namespace Leaper
