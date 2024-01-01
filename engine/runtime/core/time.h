#pragma once

namespace Leaper
{
    class Time
    {
    public:
        static const float GetDeltaTime() { return m_delta_time; }
    private:
        static void Update();
        static float m_delta_time;
        friend class Application;
    };

} // namespace Leaper
