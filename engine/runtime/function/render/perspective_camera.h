#pragma once
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include <glm/glm.hpp>

namespace Leaper
{
    class PerspectiveCamera
    {
    public:
        PerspectiveCamera(float width, float height, float fov = 1.0f) : m_width(width), m_height(height), m_fov(fov)
        {
            RecalculateViewMat();
        }

        void UpdateProjection();
        void UpdateView();

        void SetPosition(glm::vec3 pos)
        {
            m_position = pos;
            RecalculateViewMat();
        }
        void SetYaw(float yaw)
        {
            m_yaw = yaw;
            RecalculateViewMat();
        }
        void SetPitch(float pitch)
        {
            m_pitch = pitch;
            RecalculateViewMat();
        }

        const float GetYaw()
        {
            return m_yaw;
        }
        const float GetPitch()
        {
            return m_pitch;
        }
        const glm::vec3& GetFront()
        {
            return m_front;
        }
        const glm::vec3& GetRight()
        {
            return m_right;
        }

        const glm::mat4& GetView();
        const glm::mat4& GetProjection();

    protected:
        void RecalculateViewMat();

        const glm::vec3 world_up = { 0.0f, 1.0f, 0.0f };

        glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
        glm::vec3 m_targrt;
        glm::vec3 m_direction;
        glm::vec3 m_up;
        glm::vec3 m_right;
        glm::vec3 m_front;

        float m_yaw = -90.0f, m_pitch = 0.0f;
        float m_fov = 45.0f;
        float m_width, m_height;

        glm::mat4 m_projection;
        glm::mat4 m_view;
    };
}  // namespace Leaper