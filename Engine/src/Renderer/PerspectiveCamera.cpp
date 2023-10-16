#include "Core/Kareeem.h"
#include "PerspectiveCamera.h"

namespace Karem {

    void PerspectiveCamera::RecalculateProjectionMatrix()
    {
        m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
    }

    void PerspectiveCamera::RecalculateViewProjectionMatrix()
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
            glm::rotate(glm::mat4(1.0f), m_Rotation.x, glm::vec3(1, 0, 0)) *
            glm::rotate(glm::mat4(1.0f), m_Rotation.y, glm::vec3(0, 1, 0)) *
            glm::rotate(glm::mat4(1.0f), m_Rotation.z, glm::vec3(0, 0, 1));

        m_ViewMatrix = glm::inverse(transform);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

}