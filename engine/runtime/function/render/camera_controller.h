#pragma once
#include "orthographic_camera.h"

#include "core/events/events.h"
#include "core/events/application_event.h"
#include "core/events/mouse_event.h"
#include "core/events/key_event.h"

namespace Leaper
{
    class CameraController
    {
    public:
        CameraController(float ratio);

        void OnUpdate();
        void OnEvent(Leaper::Event& e);
        bool OnWindowResize(Leaper::WindowResizeEvent& event);
        bool OnMouseScrolledEvent(Leaper::MouseScrolledEvent& event);
        bool OnMouseMovedEvent(Leaper::MouseMovedEvent& event);
        void OnResize(float width, float height);

        inline const float GetZoomLevel() { return m_zoom_level; }
        inline Leaper::OrthgraphicCamera& GetCamera() { return m_camera; }
    public:
        float m_camera_speed = 2.5f;
        float m_zoom_level;
        float m_aspect_ratio;
        float m_ratio = 1.0f;
    private:
        float m_left,m_right, m_bottom, m_top;
        Leaper::OrthgraphicCamera m_camera;
        glm::vec3 m_camera_position = glm::vec3(0.0f, 0.0f, 0.0f);
    };
}
