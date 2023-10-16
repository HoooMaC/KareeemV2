#pragma once

#include "Event/AppEvent.h"
#include "Event/MouseEvent.h"

#include "Core/TimeStep.h"

#include <glm/glm.hpp>

namespace Karem {

    class PerspectiveCamera
    {
    public:
        PerspectiveCamera() = default;
        PerspectiveCamera(float aspectRatio, float fov, float nearClip, float farClip)
            : m_AspectRatio(aspectRatio), m_FOV(fov), m_NearClip(nearClip), m_FarClip(farClip)
        {
            RecalculateProjectionMatrix();
            m_ViewMatrix = glm::mat4(1.0f);
            RecalculateViewProjectionMatrix();
        }

        const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

        glm::vec3& GetPosition() { return m_Position; }
        void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewProjectionMatrix(); }

        float& GetRotationX() { return m_Rotation.x; }
        float& GetRotationY() { return m_Rotation.y; }
        float& GetRotationZ() { return m_Rotation.z; }
        void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; RecalculateViewProjectionMatrix(); }

        void SetFOV(float fov)
        {
            m_FOV = fov;
            RecalculateProjectionMatrix();
            RecalculateViewProjectionMatrix();
        }

        void SetAspectRatio(float aspectRatio)
        {
            m_AspectRatio = aspectRatio;
            RecalculateProjectionMatrix();
            RecalculateViewProjectionMatrix();
        }

        void SetNearClip(float nearClip)
        {
            m_NearClip = nearClip;
            RecalculateProjectionMatrix();
            RecalculateViewProjectionMatrix();
        }

        void SetFarClip(float farClip)
        {
            m_FarClip = farClip;
            RecalculateProjectionMatrix();
            RecalculateViewProjectionMatrix();
        }

        void Update(TimeStep ts) {}
        void EventHandler(Event& e) {}

    private:
        void RecalculateProjectionMatrix();
        void RecalculateViewProjectionMatrix();

    private:
        float m_AspectRatio;
        float m_FOV;
        float m_NearClip;
        float m_FarClip;

        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewProjectionMatrix;

        glm::vec3 m_Position = glm::vec3(0.0f);
        glm::vec3 m_Rotation = glm::vec3(0.0f);
    };


}