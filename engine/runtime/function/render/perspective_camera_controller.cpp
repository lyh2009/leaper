#include "core/base.h"
#include "core/time.h"
#include "function/input/input.h"
#include "function/input/key_codes.h"
#include "imgui.h"
#include "perspective_camera_controller.h"

#include <glfw/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Leaper
{
    using namespace Leaper;
    EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip) : m_fov(fov), m_aspect_ratio(aspectRatio), m_near_clip(nearClip), m_far_clip(farClip)
    {
        m_projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
        UpdateView();
    }

    void EditorCamera::UpdateProjection()
    {
        m_aspect_ratio = m_viewport_width / m_viewport_height;
        m_projection   = glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_near_clip, m_far_clip);
    }

    void EditorCamera::UpdateView()
    {
        // m_yaw = m_pitch = 0.0f; // Lock the camera's rotation
        m_position = CalculatePosition();

        glm::quat orientation = GetOrientation();
        m_view_matrix         = glm::translate(glm::mat4(1.0f), m_position) * glm::toMat4(orientation);
        m_view_matrix         = glm::inverse(m_view_matrix);
    }

    std::pair<float, float> EditorCamera::PanSpeed() const
    {
        float x       = std::min(m_viewport_width / 1000.0f, 2.4f);  // max = 2.4f
        float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

        float y       = std::min(m_viewport_height / 1000.0f, 2.4f);  // max = 2.4f
        float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

        return { xFactor, yFactor };
    }

    float EditorCamera::RotationSpeed() const
    {
        return 0.8f;
    }

    float EditorCamera::ZoomSpeed() const
    {
        float distance = m_distance * 0.2f;
        distance       = std::max(distance, 0.0f);
        float speed    = distance * distance;
        speed          = std::min(speed, 100.0f);  // max speed = 100
        return speed;
    }

    void EditorCamera::OnUpdate()
    {
        const glm::vec2& mouse{ ImGui::GetMousePos().x, ImGui::GetMousePos().y };
        glm::vec2 delta         = (mouse - m_initia_mouse_position) * Time::GetDeltaTime();
        m_initia_mouse_position = mouse;
        if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
            MouseRotate(delta);
        else if (ImGui::IsMouseDown(ImGuiMouseButton_Middle))
            MouseZoom(delta.y);

        float speed = 10.f;
        if (Input::IsKeyDown(LP_KEY_W)) m_focal_point += GetForwardDirection() * speed * Time::GetDeltaTime();
        if (Input::IsKeyDown(LP_KEY_S)) m_focal_point -= GetForwardDirection() * speed * Time::GetDeltaTime();
        if (Input::IsKeyDown(LP_KEY_A)) m_focal_point -= GetRightDirection() * speed * Time::GetDeltaTime();
        if (Input::IsKeyDown(LP_KEY_D)) m_focal_point += GetRightDirection() * speed * Time::GetDeltaTime();

        UpdateView();
    }

    void EditorCamera::OnEvent(Leaper::Event& e)
    {
        Leaper::EventDispatcher dispatcher(e);
        dispatcher.Dispatch<Leaper::MouseScrolledEvent>(LP_BIND_EVENT_FN(Leaper::EditorCamera::OnMouseScroll));
    }

    bool EditorCamera::OnMouseScroll(Leaper::MouseScrolledEvent& e)
    {
        float delta = e.GetYOffset() * 0.1f;
        MouseZoom(delta);
        UpdateView();
        return false;
    }

    void EditorCamera::MousePan(const glm::vec2& delta)
    {
        auto [x_speed, y_speed] = PanSpeed();
        m_focal_point += -GetRightDirection() * delta.x * x_speed * m_distance;
        m_focal_point += GetUpDirection() * delta.y * y_speed * m_distance;
    }

    void EditorCamera::MouseRotate(const glm::vec2& delta)
    {
        float yaw_sign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
        m_yaw += yaw_sign * delta.x * RotationSpeed();
        m_pitch += delta.y * RotationSpeed();
    }

    void EditorCamera::MouseZoom(float delta)
    {
        m_distance -= delta * ZoomSpeed();
        if (m_distance < 1.0f)
        {
            m_focal_point += GetForwardDirection();
            m_distance = 1.0f;
        }
    }

    glm::vec3 EditorCamera::GetUpDirection() const
    {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::vec3 EditorCamera::GetRightDirection() const
    {
        return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::vec3 EditorCamera::GetForwardDirection() const
    {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
    }

    glm::vec3 EditorCamera::CalculatePosition() const
    {
        return m_focal_point - GetForwardDirection() * m_distance;
    }

    glm::quat EditorCamera::GetOrientation() const
    {
        return glm::quat(glm::vec3(-m_pitch, -m_yaw, 0.0f));
    }

}  // namespace Leaper
