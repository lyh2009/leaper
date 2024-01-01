#include "game_camera.h"
#include "lppch.h"

Leaper::GameCamera::GameCamera(float ratio)
    : m_camera(-ratio * m_zoom_level, ratio * m_zoom_level, -m_zoom_level, m_zoom_level)
{
    m_ratio = ratio;
}

void Leaper::GameCamera::OnUpdate()
{
    m_camera.SetPosition(m_position);
}

void Leaper::GameCamera::OnResize(float width, float height)
{
    m_width = width;
    m_height = height;
    m_ratio = width / height;
    m_camera.SetProjectionMat(-m_ratio * m_zoom_level, m_ratio * m_zoom_level, -m_zoom_level, m_zoom_level);
}

void Leaper::GameCamera::OnEvent(Leaper::Event &e)
{
    Leaper::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<Leaper::WindowResizeEvent>(LP_BIND_EVENT_FN(GameCamera::OnWindowResize));
}

bool Leaper::GameCamera::OnWindowResize(Leaper::WindowResizeEvent &event)
{
    m_ratio = event.GetWidth() / event.GetHeight();
    m_camera.SetProjectionMat(-m_ratio * m_zoom_level, m_ratio * m_zoom_level, -m_zoom_level, m_zoom_level);
    return false;
}
