#include "time.h"
#include <GLFW/glfw3.h>

float Leaper::Time::m_delta_time = 0.0f;

void Leaper::Time::Update()
{
    static float last_frame = 0.0f;

    float current_frame = glfwGetTime();
    m_delta_time = current_frame - last_frame;
    last_frame = current_frame;
}