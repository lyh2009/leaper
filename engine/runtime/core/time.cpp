#include "time.h"
#include <GLFW/glfw3.h>

float Leaper::Time::m_delta_time = 0.0f;

float current_frame = 0.0f;
float last_frame = 0.0f;

void Leaper::Time::Begin()
{
    current_frame = glfwGetTime();
}

void Leaper::Time::End()
{
    m_delta_time = current_frame - last_frame;
    last_frame = current_frame;
}