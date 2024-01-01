#pragma once
#include "orthographic_camera.h"

#include "core/events/application_event.h"

namespace Leaper
{
    class GameCamera
    {
    public:
        GameCamera(float ratio);

        void OnUpdate();
        void OnResize(float width, float height);
        void OnEvent(Leaper::Event& e);
        const float GetRatio() const { return m_ratio; } 
        const float GetWidth() const { return m_width; } 
        const float GetHeight() const { return m_height; } 
        const float GetZoomLevel() const { return m_zoom_level; } 
        const glm::vec3 GetPosition() const { return m_position; } 
        Leaper::OrthgraphicCamera& GetCamera() { return m_camera; }
    private:
        bool OnWindowResize(Leaper::WindowResizeEvent& event);
    private:
        float m_ratio = 1.0f;
        float m_zoom_level = 1.0f;
        float m_height, m_width;
        glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f);
        Leaper::OrthgraphicCamera m_camera;
    };
} // namespace Leaper