#include "Core/Kareeem.h"
#include "PerspectiveCamera.h"

namespace Karem {

    void PerspectiveCamera::RecalculateProjectionMatrix(float aspectRatio)
    {
        m_ProjectionMatrix = glm::perspective(m_VerticalFOV, aspectRatio, m_NearClip, m_FarClip);
    }

}