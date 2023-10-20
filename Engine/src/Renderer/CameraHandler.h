#pragma once

#include "Renderer/OrthographicCamera.h"
#include "Renderer/PerspectiveCamera.h"

#include "Event/Event.h"

#include "Core/TimeStep.h"

namespace Karem {


	class CameraHandler
	{
		enum class Type : uint8_t { None = 0, Orthographic, Perspective};
	public:
		CameraHandler() = default;
		CameraHandler(const OrthographicCamera& camera)
			: m_OrthographicCamera(camera) { m_CurrentCammeraType = Type::Orthographic; }
		CameraHandler(const PerspectiveCamera& camera)
			: m_PerspectiveCamera(camera) { m_CurrentCammeraType = Type::Perspective; }

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
				case Type::None:         ENGINE_DEBUG("The camera hasn't been setted"); return  m_OrthographicCamera.GetViewProjectionMatrix();
				case Type::Orthographic: return m_OrthographicCamera.GetViewProjectionMatrix();
				case Type::Perspective:  return m_PerspectiveCamera.GetViewProjectionMatrix();
			}
			ENGINE_ASSERT(false, "Invalid Camera Type");
			return  m_OrthographicCamera.GetViewProjectionMatrix();
		}

		void* GetCamera()
		{
			switch (m_CurrentCammeraType)
			{
			case Type::None:         ENGINE_DEBUG("The camera hasn't been setted"); return  &m_OrthographicCamera;
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
				case Type::None:         ENGINE_DEBUG("The camera hasn't been setted"); return;
				case Type::Orthographic: return m_OrthographicCamera.Update(ts);
				case Type::Perspective:  return m_PerspectiveCamera.Update(ts);
			}
			ENGINE_ASSERT(false, "Invalid Camera Type");
		}

		void EventHandler(Event& e)
		{
			switch (m_CurrentCammeraType)
			{
				case Type::None:         ENGINE_DEBUG("The camera hasn't been setted"); return;
				case Type::Orthographic: return m_OrthographicCamera.EventHandler(e);
				case Type::Perspective:  return m_PerspectiveCamera.EventHandler(e);
			}
			ENGINE_ASSERT(false, "Invalid Camera Type");

		}
	private:
		Type m_CurrentCammeraType = Type::None;
		// how about the constructor of orthographic
		OrthographicCamera m_OrthographicCamera = OrthographicCamera({ 16,9 }, 1); // this is temporary, the default camera
		// TODO : Add Perspective camera
		PerspectiveCamera m_PerspectiveCamera = PerspectiveCamera(1.77, glm::radians(45.0f), 0.001f, 1000.0f);
	};

}