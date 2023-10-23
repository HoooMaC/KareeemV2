#include "Core/Kareeem.h"
#include "PerspectiveCamera.h"

namespace Karem {

    void PerspectiveCamera::RecalculateProjectionMatrix(float aspectRatio)
    {
        float newProjectionHeight = glm::tan(0.5f * m_VerticalFOV) * m_NearClip;
        float newProjectionWidth = aspectRatio * newProjectionHeight;

        m_ProjectionMatrix = glm::frustum(-newProjectionWidth, newProjectionWidth, -newProjectionHeight, newProjectionHeight, m_NearClip, m_FarClip);
    }

}