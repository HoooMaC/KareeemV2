#pragma once

#include "Renderer/OrthographicCamera.h"
#include "Renderer/PerspectiveCamera.h"

#include "Event/Event.h"

#include "Core/TimeStep.h"

namespace Karem {

	class CameraHandler
	{
		enum class Type : uint8_t { Orthographic = 0, Perspective = 1 };
	public:
		CameraHandler();
		CameraHandler(const OrthographicCamera& orthographicCamera);
		CameraHandler(const PerspectiveCamera& perspectiveCamera);
		~CameraHandler();
		// field for the camera type functions
	public:
		// should consider if any camera hasn't been setted
		void ChangeCameraType() { m_CurrentCammeraType = static_cast<Type>((uint8_t)m_CurrentCammeraType ^ 1); }

		const char* GetCameraStringType() const
		{
			switch (m_CurrentCammeraType)
			{
				case Type::Perspective: return "Perspective";
				case Type::Orthographic: break;
			}
			return  "Orthographic";
		}

		void SetAspectRatio(float aspectRatio)
		{
			m_AspectRatio = aspectRatio;
			m_OrthographicCamera.RecalculateProjectionMatrix(m_AspectRatio);
			m_PerspectiveCamera.RecalculateProjectionMatrix(m_AspectRatio);
		}

		float GetAspectRatio() const { return m_AspectRatio; }

		void SetOrthographicCamera(const OrthographicCamera& camera) { m_OrthographicCamera = camera; }
		void SetPerspectiveCamera(const PerspectiveCamera& camera) { m_PerspectiveCamera = camera; }

		void SetTypeToOrthographic() { m_CurrentCammeraType = Type::Orthographic; }
		void SetTypeToPerspective() { m_CurrentCammeraType = Type::Perspective; }

		bool IsOrthographic() const { return m_CurrentCammeraType == Type::Orthographic; }
		bool IsPerspective() const { return m_CurrentCammeraType == Type::Perspective; }

		// TEMPORARY  FDNSFJAK
		const glm::mat4& GetCameraProjectionMatrix() const
		{
			switch (m_CurrentCammeraType)
			{
				case Type::Perspective:  return m_PerspectiveCamera.GetProjectionMatrix();
				case Type::Orthographic: break;
			}
			return m_OrthographicCamera.GetProjectionMatrix();
		}

		// TEMPORARY 
		void* GetCamera()
		{
			switch (m_CurrentCammeraType)
			{
				case Type::Perspective:  return &m_PerspectiveCamera;
				case Type::Orthographic: break;
			}
			return &m_OrthographicCamera;
		}

	public:
		// TEMPORARY 
		std::tuple<float, float, float, float> GetCameraBounds()
		{
			switch (m_CurrentCammeraType)
			{
				case Type::Orthographic:
				{
					float right = (m_OrthographicCamera.m_Size * m_AspectRatio) / 2;
					float left = -right;
					float top = m_OrthographicCamera.m_Size / 2;
					float bottom = -top;
					return { left, right, bottom, top };
				}
				case Type::Perspective:
				{
					// TEMPORARY 
					ENGINE_ASSERT(false, "Invalid Camera Type");
				}
			}
			ENGINE_ASSERT(false, "Invalid Camera Type");
		}

		float GetCameraNear()
		{
			switch (m_CurrentCammeraType)
			{
				case Type::Perspective:  return m_PerspectiveCamera.GetPerspectiveNear();
				case Type::Orthographic: break;
			}
			return m_OrthographicCamera.GetOrthographicNear();
		}

		void SetCameraNear(float nearClip)
		{
			switch (m_CurrentCammeraType)
			{
				case Type::Perspective:  return m_PerspectiveCamera.SetPerspectiveNearClip(nearClip, m_AspectRatio);
				case Type::Orthographic: break;
			}
			return m_OrthographicCamera.SetOrthographicNear(nearClip, m_AspectRatio);
		}

		float GetCameraFar()
		{
			switch (m_CurrentCammeraType)
			{
				case Type::Perspective:  return m_PerspectiveCamera.GetPerspectiveFar();
				case Type::Orthographic: break;
			}
			return m_OrthographicCamera.GetOrthographicFar();
		}

		void SetCameraFar(float farClip)
		{
			switch (m_CurrentCammeraType)
			{
				case Type::Perspective:  return m_PerspectiveCamera.SetPerspectiveFarClip(farClip, m_AspectRatio);
				case Type::Orthographic: break;
			}
			return m_OrthographicCamera.SetOrthographicFar(farClip, m_AspectRatio);
		}

	// field for orthographic, some code here maybe temporary
	public:
		float GetOrthographicSize() const { return m_OrthographicCamera.m_Size; }
		void SetOrthographicSize(float size) { m_OrthographicCamera.m_Size = size; m_OrthographicCamera.RecalculateProjectionMatrix(m_AspectRatio); }

	public:
		float GetPerspectiveVerticalFOV() const { return m_PerspectiveCamera.GetPerspectiveVerticalFOV(); }
		void SetPerspectiveVerticalFOV(float fov) { return m_PerspectiveCamera.SetPerspectiveVerticalFOV(fov, m_AspectRatio); }

	private:
		// TODO : the default aspect ratio should be set 
		// according to the viewport size
		//float m_AspectRatio = ImGui::GetWindowSize().x / ImGui::GetWindowSize().y; // what have i done???
		float m_AspectRatio;

		Type m_CurrentCammeraType;
		OrthographicCamera m_OrthographicCamera;
		PerspectiveCamera m_PerspectiveCamera;
	};

}