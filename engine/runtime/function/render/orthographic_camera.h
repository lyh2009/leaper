#pragma once
#include "glm/geometric.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Leaper
{
    class OrthgraphicCamera
    {
    public:
        OrthgraphicCamera(float left, float right, float bottom, float top);
        void SetPosition(glm::vec3 position)
        {
            m_position = position;
            RecalculateViewMat();
        }
        void SetRotation(float rotation)
        {
            m_rotation = rotation;
            RecalculateViewMat();
        }
        void SetContentBroswerionMat(float left, float right, float bottom, float top)
        {
            m_ContentBroswerion_mat = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
            RecalculateViewMat();
        }

        inline const glm::mat4& GetContentBroswerionMat() const
        {
            return m_ContentBroswerion_mat;
        }
        inline const glm::mat4& GetViewMat() const
        {
            return m_view_mat;
        }
        inline const glm::mat4& GetViewContentBroswerionMat() const
        {
            return m_view_ContentBroswerion_mat;
        }
        inline const glm::vec3& GetPosition() const
        {
            return m_position;
        }
        const glm::vec3& Front() const
        {
            glm::vec3 front;
            front.x = cos(glm::radians(-90.0f)) * cos(glm::radians(0.0f));
            front.y = sin(glm::radians(0.0f));
            front.z = sin(glm::radians(-90.0f)) * cos(glm::radians(0.0f));
            front   = glm::normalize(front);
            return front;
        }

    private:
        void RecalculateViewMat();

    private:
        glm::mat4 m_ContentBroswerion_mat      = glm::mat4(1.0f);
        glm::mat4 m_view_mat                   = glm::mat4(1.0f);
        glm::mat4 m_view_ContentBroswerion_mat = glm::mat4(1.0f);

        glm::vec3 m_position = glm::vec3(0);
        float m_rotation     = 0.0f;
    };
}  // namespace Leaper
