#pragma once

#include "Event/AppEvent.h"
#include "Event/MouseEvent.h"

#include "Core/TimeStep.h"

#include <glm/glm.hpp>

namespace Karem {

    class CameraHandler;

    class PerspectiveCamera
    {
        friend class CameraHandler;
    public:
        PerspectiveCamera() = default;
        PerspectiveCamera(float aspectRatio, float fov, float nearClip, float farClip)
            : m_VerticalFOV(fov), m_NearClip(nearClip), m_FarClip(farClip)
        {
            RecalculateProjectionMatrix(aspectRatio);
        }

        const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

        float GetPerspectiveVerticalFOV() const { return m_VerticalFOV; }
        void SetPerspectiveVerticalFOV(float fov, float aspectRatio) { m_VerticalFOV = fov; RecalculateProjectionMatrix(aspectRatio); }

        float GetPerspectiveNear() const { return m_NearClip; }
        void SetPerspectiveNearClip(float nearClip, float aspectRatio) { m_NearClip = nearClip; RecalculateProjectionMatrix(aspectRatio); }

        float GetPerspectiveFar() const { return m_FarClip; }
        void SetPerspectiveFarClip(float farClip, float aspectRatio) { m_FarClip = farClip; RecalculateProjectionMatrix(aspectRatio); }

    private:
        void RecalculateProjectionMatrix(float aspectRatio);

    private:
        float m_VerticalFOV = glm::radians(45.0f);
        float m_NearClip = 0.001f;
        float m_FarClip = 1000.0f;

        glm::mat4 m_ProjectionMatrix;
    };


}