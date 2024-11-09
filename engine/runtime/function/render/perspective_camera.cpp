#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include "perspective_camera.h"
#include "perspective_camera_controller.h"
#include <glm/gtx/quaternion.hpp>


const glm::mat4& Leaper::PerspectiveCamera::GetView()
{
    return m_view;
}

const glm::mat4& Leaper::PerspectiveCamera::GetContentBroswerion()
{
    return m_ContentBroswerion;
}

void Leaper::PerspectiveCamera::UpdateContentBroswerion()
{
    m_ContentBroswerion = glm::perspective(glm::radians(m_fov), m_width / m_height, 0.1f, 1000.0f);
}

void Leaper::PerspectiveCamera::UpdateView()
{
    m_view = glm::lookAt(m_position, m_position + m_front, m_up);
}

void Leaper::PerspectiveCamera::RecalculateViewMat()
{
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);

    m_right = glm::normalize(glm::cross(m_front, world_up));
    m_up    = glm::normalize(glm::cross(m_right, m_front));
}