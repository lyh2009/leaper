#include "lppch.h"
#include "orthographic_camera.h"


Leaper::OrthgraphicCamera::OrthgraphicCamera(float left, float right, float bottom, float top) : m_ContentBroswerion_mat(glm::ortho(left, right, bottom, top, -1.0f, 1.0f))
{
    m_view_ContentBroswerion_mat = m_ContentBroswerion_mat * m_view_mat;
    RecalculateViewMat();
}

void Leaper::OrthgraphicCamera::RecalculateViewMat()
{
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) * glm::rotate(glm::mat4(1.0f), m_rotation, glm::vec3(0, 0, 1));

    m_view_mat                   = glm::inverse(transform);
    m_view_ContentBroswerion_mat = m_ContentBroswerion_mat * m_view_mat;
}