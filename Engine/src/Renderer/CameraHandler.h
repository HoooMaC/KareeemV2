#pragma once

#include "Renderer/OrthographicCamera.h"

#include "Event/Event.h"

#include "Core/TimeStep.h"

namespace Karem {

	class CameraHandler
	{
	// this is temporary, the enum should private --PROBABLY--	
	public:
		enum class CameraType : uint8_t { None = 0, Orthographic, Perspective};
	public:
		CameraHandler() = default;
		CameraHandler(const OrthographicCamera& camera)
			: m_OrthographicCamera(camera) {}
		void SetOrthographicCamera(const OrthographicCamera& camera) { m_OrthographicCamera = camera; }
		//void SetPerspectiveCamera(const PerspectiveCamera& camera) { m_PerspectiveCamera = camera; }

		void SetCurrrentCamera(CameraType type) { m_CurrentCammeraType = type; }

		// this is temporary
		OrthographicCamera GetCamera(CameraType type)
		{
			switch (m_CurrentCammeraType)
			{
				case CameraType::None:         ENGINE_DEBUG("The camera hasn't been setted"); return m_OrthographicCamera;
				case CameraType::Orthographic: return m_OrthographicCamera;
				case CameraType::Perspective:  ENGINE_ASSERT(false, "Perspective Camera Type hasn't supported yet"); return m_OrthographicCamera;
			}
			ENGINE_ASSERT(false, "Invalid Camera Type");
			return m_OrthographicCamera;
		}
	public:
		void Update(TimeStep ts)
		{
			switch (m_CurrentCammeraType)
			{
				case CameraType::None:         ENGINE_DEBUG("The camera hasn't been setted"); return;
				case CameraType::Orthographic: return m_OrthographicCamera.Update(ts);
				case CameraType::Perspective:  return;
			}
			ENGINE_ASSERT(false, "Invalid Camera Type");
		}

		void EventHandler(Event& e)
		{
			switch (m_CurrentCammeraType)
			{
				case CameraType::None:         ENGINE_DEBUG("The camera hasn't been setted"); return;
				case CameraType::Orthographic: return m_OrthographicCamera.EventHandler(e);
				case CameraType::Perspective:  return;
			}
			ENGINE_ASSERT(false, "Invalid Camera Type");

		}
	private:
		CameraType m_CurrentCammeraType = CameraType::None;
		// how about the constructor of orthographic
		OrthographicCamera m_OrthographicCamera = Karem::OrthographicCamera({ 16,9 }, 1); // this is temporary, the default camera
		// TODO : Add Perspective camera
	};

}