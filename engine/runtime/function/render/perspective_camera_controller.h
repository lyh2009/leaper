#pragma once

#include "core/events/events.h"
#include "core/events/mouse_event.h"

#include <glm/glm.hpp>

namespace Leaper
{

    class EditorCamera
    {
    public:
        EditorCamera() = default;
        EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

        void OnUpdate();
        void OnEvent(Leaper::Event& e);

        inline float GetDistance() const
        {
            return m_distance;
        }
        inline void SetDistance(float distance)
        {
            m_distance = distance;
        }

        inline void SetViewportSize(float width, float height)
        {
            m_viewport_width  = width;
            m_viewport_height = height;
            UpdateProjection();
        }

        const glm::mat4& GetViewMatrix() const
        {
            return m_view_matrix;
        }
        glm::mat4 GetViewProjection() const
        {
            return m_projection * m_view_matrix;
        }
        glm::mat4 GetProjection() const
        {
            return m_projection;
        }

        glm::vec3 GetUpDirection() const;
        glm::vec3 GetRightDirection() const;
        glm::vec3 GetForwardDirection() const;
        const glm::vec3& GetPosition() const
        {
            return m_position;
        }
        glm::quat GetOrientation() const;

        float GetPitch() const
        {
            return m_pitch;
        }
        float GetYaw() const
        {
            return m_yaw;
        }

    private:
        void UpdateProjection();
        void UpdateView();

        bool OnMouseScroll(Leaper::MouseScrolledEvent& e);

        void MousePan(const glm::vec2& delta);
        void MouseRotate(const glm::vec2& delta);
        void MouseZoom(float delta);

        glm::vec3 CalculatePosition() const;

        std::pair<float, float> PanSpeed() const;
        float RotationSpeed() const;
        float ZoomSpeed() const;

    private:
        float m_fov = 45.0f, m_aspect_ratio = 1.778f, m_near_clip = 0.1f, m_far_clip = 1000.0f;

        glm::mat4 m_projection;

        glm::mat4 m_view_matrix;
        glm::vec3 m_position    = { 0.0f, 0.0f, 0.0f };
        glm::vec3 m_focal_point = { 0.0f, 0.0f, 0.0f };

        glm::vec2 m_initia_mouse_position = { 0.0f, 0.0f };

        float m_distance = 10.0f;
        float m_pitch = 0.0f, m_yaw = 0.0f;

        float m_viewport_width = 1024, m_viewport_height = 648;
    };

}  // namespace Leaper
