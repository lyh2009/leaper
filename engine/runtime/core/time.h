#pragma once
#include <string>

namespace Leaper
{
    class Time
    {
    public:
        static const float GetDeltaTime() { return m_delta_time; }
        static const std::string GetTime(const std::string &format);

    private:
        static void Begin();
        static void End();
        static float m_delta_time;
        friend class Application;
    };

} // namespace Leaper
