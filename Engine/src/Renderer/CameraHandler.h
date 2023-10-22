#pragma once

#include "Renderer/OrthographicCamera.h"
#include "Renderer/PerspectiveCamera.h"

#include "Event/Event.h"

#include "Core/TimeStep.h"

namespace Karem {

	class CameraHandler
	{
		enum class Type : uint8_t { Orthographic = 0, Perspective = 1};
	public:
		CameraHandler() = default;
		CameraHandler(const OrthographicCamera& camera)
			: m_OrthographicCamera(camera) { m_CurrentCammeraType = Type::Orthographic; }
		CameraHandler(const PerspectiveCamera& camera)
			: m_PerspectiveCamera(camera) { m_CurrentCammeraType = Type::Perspective; }
	// field for the camera type functions
	public:
		// should consider if any camera hasn't been setted
		void ChangeCameraType() { m_CurrentCammeraType = static_cast<Type>((uint8_t)m_CurrentCammeraType ^ 1); }

		const char* GetCameraStringType() const 
		{ 
			switch (m_CurrentCammeraType)
			{
				case Type::Orthographic: return "Orthographic";
				case Type::Perspective: return "Perspective";
			}
			ENGINE_ASSERT(false, "Invalid Camera Type");
			return  "Orthographic";
		}

		void SetOrthographicCamera(const OrthographicCamera& camera) { m_OrthographicCamera = camera; }
		void SetPerspectiveCamera(const PerspectiveCamera& camera) { m_PerspectiveCamera = camera; }

		void SetToOrthographic() { m_CurrentCammeraType = Type::Orthographic; }
		void SetToPerspective() { m_CurrentCammeraType = Type::Perspective; }

		bool IsOrthographic() const { return m_CurrentCammeraType == Type::Orthographic; }
		bool IsPerspective() const { return m_CurrentCammeraType == Type::Perspective; }

		// this is temporary
		const glm::mat4& GetViewProjection(Type type) const
		{
			switch (m_CurrentCammeraType)
			{
				case Type::Orthographic: return m_OrthographicCamera.GetViewProjectionMatrix();
				case Type::Perspective:  return m_PerspectiveCamera.GetViewProjectionMatrix();
			}
			// this is can be replaced with std::unreachable int c++23
			ENGINE_ASSERT(false, "Invalid Camera Type");
			return  m_OrthographicCamera.GetViewProjectionMatrix();
		}

		// this is cam be replaced with std::variant or maybe std::optional
		void* GetCamera()
		{
			switch (m_CurrentCammeraType)
			{
				case Type::Orthographic: return &m_OrthographicCamera;
				case Type::Perspective:  return &m_PerspectiveCamera;
			}
			ENGINE_ASSERT(false, "Invalid Camera Type");
			return  &m_OrthographicCamera;
		}
	public:
		void Update(TimeStep ts)
		{
			switch (m_CurrentCammeraType)
			{
				case Type::Orthographic: return m_OrthographicCamera.Update(ts);
				case Type::Perspective:  return m_PerspectiveCamera.Update(ts);
			}
			// this is can be replaced with std::unreachable int c++23
			ENGINE_ASSERT(false, "Invalid Camera Type");
		}

		void EventHandler(Event& e)
		{
			switch (m_CurrentCammeraType)
			{
				case Type::Orthographic: return m_OrthographicCamera.EventHandler(e);
				case Type::Perspective:  return m_PerspectiveCamera.EventHandler(e);
			}
			// this is can be replaced with std::unreachable int c++23
			ENGINE_ASSERT(false, "Invalid Camera Type");
		}
	private:
		Type m_CurrentCammeraType;
		OrthographicCamera m_OrthographicCamera;
		PerspectiveCamera m_PerspectiveCamera;
	};

}