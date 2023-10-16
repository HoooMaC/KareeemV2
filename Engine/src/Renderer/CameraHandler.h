#pragma once

#include "Renderer/OrthographicCamera.h"
#include "Renderer/PerspectiveCamera.h"

#include "Event/Event.h"

#include "Core/TimeStep.h"

namespace Karem {

	enum class CameraType : uint8_t { None = 0, Orthographic, Perspective};

	class CameraHandler
	{
	public:
		CameraHandler() = default;
		CameraHandler(const OrthographicCamera& camera)
			: m_OrthographicCamera(camera) { m_CurrentCammeraType = CameraType::Orthographic; }
		CameraHandler(const PerspectiveCamera& camera)
			: m_PerspectiveCamera(camera) { m_CurrentCammeraType = CameraType::Perspective; }

		void SetOrthographicCamera(const OrthographicCamera& camera) { m_OrthographicCamera = camera; }
		void SetPerspectiveCamera(const PerspectiveCamera& camera) { m_PerspectiveCamera = camera; }

		void SetCurrrentCamera(CameraType type) { m_CurrentCammeraType = type; }
		CameraType GetCurrentCameraType() const { return m_CurrentCammeraType; }

		// this is temporary
		void* GetCamera(CameraType type)
		{
			switch (m_CurrentCammeraType)
			{
				case CameraType::None:         ENGINE_DEBUG("The camera hasn't been setted"); return &m_OrthographicCamera;
				case CameraType::Orthographic: return &m_OrthographicCamera;
				case CameraType::Perspective:  return &m_PerspectiveCamera;
			}
			ENGINE_ASSERT(false, "Invalid Camera Type");
			return &m_OrthographicCamera;
		}
	public:
		void Update(TimeStep ts)
		{
			switch (m_CurrentCammeraType)
			{
				case CameraType::None:         ENGINE_DEBUG("The camera hasn't been setted"); return;
				case CameraType::Orthographic: return m_OrthographicCamera.Update(ts);
				case CameraType::Perspective:  return m_PerspectiveCamera.Update(ts);
			}
			ENGINE_ASSERT(false, "Invalid Camera Type");
		}

		void EventHandler(Event& e)
		{
			switch (m_CurrentCammeraType)
			{
				case CameraType::None:         ENGINE_DEBUG("The camera hasn't been setted"); return;
				case CameraType::Orthographic: return m_OrthographicCamera.EventHandler(e);
				case CameraType::Perspective:  return m_PerspectiveCamera.EventHandler(e);
			}
			ENGINE_ASSERT(false, "Invalid Camera Type");

		}
	private:
		CameraType m_CurrentCammeraType = CameraType::None;
		// how about the constructor of orthographic
		OrthographicCamera m_OrthographicCamera = OrthographicCamera({ 16,9 }, 1); // this is temporary, the default camera
		// TODO : Add Perspective camera
		PerspectiveCamera m_PerspectiveCamera = PerspectiveCamera(1920, glm::radians(45.0f), 0.001f, 1000.0f);
	};

}