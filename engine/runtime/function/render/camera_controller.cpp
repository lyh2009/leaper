#include "camera_controller.h"
#include "core/events/mouse_event.h"
#include "function/input/key_codes.h"
#include "function/input/mouse_codes.h"
#include "imgui.h"
#include "lppch.h"

#include "core/time.h"
#include "function/application/application.h"
#include "function/input/input.h"

Leaper::CameraController::CameraController(float ratio) : m_camera(-ratio * m_zoom_level, ratio * m_zoom_level, -m_zoom_level, m_zoom_level)
{
    m_ratio      = ratio;
    m_zoom_level = 1.0f;
}

void Leaper::CameraController::OnUpdate()
{
    if (Leaper::Input::IsKeyDown(LP_KEY_A))
    {
        m_camera_position.x -= m_camera_speed * Leaper::Time::GetDeltaTime();
    }
    if (Leaper::Input::IsKeyDown(LP_KEY_D))
    {
        m_camera_position.x += m_camera_speed * Leaper::Time::GetDeltaTime();
    }
    if (Leaper::Input::IsKeyDown(LP_KEY_W))
    {
        m_camera_position.y += m_camera_speed * Leaper::Time::GetDeltaTime();
    }
    if (Leaper::Input::IsKeyDown(LP_KEY_S))
    {
        m_camera_position.y -= m_camera_speed * Leaper::Time::GetDeltaTime();
    }

    m_camera.SetPosition(m_camera_position);
    m_camera_speed = m_zoom_level;
}

void Leaper::CameraController::OnEvent(Leaper::Event& e)
{
    Leaper::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<Leaper::WindowResizeEvent>(LP_BIND_EVENT_FN(CameraController::OnWindowResize));
    dispatcher.Dispatch<Leaper::MouseScrolledEvent>(LP_BIND_EVENT_FN(CameraController::OnMouseScrolledEvent));
    dispatcher.Dispatch<Leaper::MouseMovedEvent>(LP_BIND_EVENT_FN(CameraController::OnMouseMovedEvent));
}

bool Leaper::CameraController::OnMouseScrolledEvent(Leaper::MouseScrolledEvent& event)
{
    m_zoom_level -= event.GetYOffset() * 0.25f;
    m_zoom_level = std::max(m_zoom_level, 0.25f);
    m_camera.SetContentBroswerionMat(-m_ratio * m_zoom_level, m_ratio * m_zoom_level, -m_zoom_level, m_zoom_level);
    return false;
}

bool Leaper::CameraController::OnMouseMovedEvent(Leaper::MouseMovedEvent& event)
{
    static float last_x = event.GetMouseX();
    static float last_y = event.GetMouseY();

    float offset_x = last_x - event.GetMouseX();
    float offset_y = last_y - event.GetMouseY();

    last_x = event.GetMouseX();
    last_y = event.GetMouseY();
    if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
    {
        // m_camera.SetPosition(m_camera_position);
        m_camera_position.x += offset_x * Time::GetDeltaTime();
        m_camera_position.y -= offset_y * Time::GetDeltaTime();
    }
    return false;
}

void Leaper::CameraController::OnResize(float width, float height)
{
    m_ratio = width / height;
    m_camera.SetContentBroswerionMat(-m_ratio * m_zoom_level, m_ratio * m_zoom_level, -m_zoom_level, m_zoom_level);
}

bool Leaper::CameraController::OnWindowResize(Leaper::WindowResizeEvent& event)
{
    if (event.GetHeight() != 0 || event.GetHeight() != 0)
    {
        m_ratio = event.GetWidth() / event.GetHeight();
        m_camera.SetContentBroswerionMat(-m_ratio * m_zoom_level, m_ratio * m_zoom_level, -m_zoom_level, m_zoom_level);
    }
    return false;
}
