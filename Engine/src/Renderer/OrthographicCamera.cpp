#include "Core/Kareeem.h"
#include "OrthographicCamera.h"

#include "Event/KeyCode.h"
#include "Event/Input.h"

#define CAMERA_DEBUG 0

namespace Karem {

	void OrthographicCamera::Update(TimeStep ts)
	{
		if (Karem::Input::IsKeyPressed(KAREM_KEY_R))
		{
			m_Rotation = 0;
			m_Position = glm::vec3(0.0f);
			RecalculateProjectionViewMatrix();
		}
		else if (Karem::Input::IsKeyPressed(KAREM_KEY_E))
		{
			m_Rotation += 180.0f * ts;
			RecalculateProjectionViewMatrix();
		}
		else if (Karem::Input::IsKeyPressed(KAREM_KEY_Q))
		{
			m_Rotation -= 180.0f * ts;
			RecalculateProjectionViewMatrix();
		}

		if (Karem::Input::IsKeyPressed(KAREM_KEY_D))
		{
			m_Position.x += 10.0f * ts;
			RecalculateProjectionViewMatrix();
		}
		else if (Karem::Input::IsKeyPressed(KAREM_KEY_A))
		{
			m_Position.x -= 10.0f * ts;
			RecalculateProjectionViewMatrix();
		}

		if (Karem::Input::IsKeyPressed(KAREM_KEY_W))
		{
			m_Position.y += 10.0f * ts;
			RecalculateProjectionViewMatrix();
		}
		else if (Karem::Input::IsKeyPressed(KAREM_KEY_S))
		{
			m_Position.y -= 10.0f * ts;
			RecalculateProjectionViewMatrix();
		}
	}

	void OrthographicCamera::EventHandler(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&OrthographicCamera::MouseScrolledEventAction, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&OrthographicCamera::WindowResizeEventAction, this, std::placeholders::_1));
	}

	bool OrthographicCamera::MouseScrolledEventAction(MouseScrolledEvent& event)
	{
		float eventScrollData = event.GetOffsetY();;
		m_Zoom = std::clamp(m_Zoom - eventScrollData, 1.0f, 100.0f);
#if CAMERA_DEBUG
		auto [Right, Left, Top, Bottom, Near, Far, AspectRatio] = GetBounds();
		ENGINE_DEBUG("{}", m_Zoom);
		ENGINE_TRACE("left {:.3f}\tright  {:.3f}", Left, Right);
		ENGINE_TRACE("Top  {:.3f}\tBottom {:.3f}", Top, Bottom);
		ENGINE_TRACE("Near {:.3f}\tFar    {:.3f}", Near, Far);
		ENGINE_TRACE("ASPRX{:.3f}\tASPRY  {:.3f}", AspectRatio.x, AspectRatio.y);
		const char* status = isFixedAspectRatio ? "True" : "False";
		ENGINE_TRACE("Stat {}", status);
#endif

		m_Bounds.RecalculateBound(m_Zoom, isFixedAspectRatio);
		RecalculateProjectionMatrix();
		RecalculateProjectionViewMatrix();
		return true;
	}

	bool OrthographicCamera::WindowResizeEventAction(WindowResizeEvent& event)
	{
		SetAspectRatio({ event.GetWidth(), event.GetHeight() });

#if CAMERA_DEBUG
		auto [Right, Left, Top, Bottom, Near, Far, AspectRatio] = GetBounds();
		ENGINE_DEBUG("{}", m_Zoom);
		ENGINE_TRACE("left {:.3f}\tright  {:.3f}", Left, Right);
		ENGINE_TRACE("Top  {:.3f}\tBottom {:.3f}", Top, Bottom);
		ENGINE_TRACE("Near {:.3f}\tFar    {:.3f}", Near, Far);
		ENGINE_TRACE("ASPRX{:.3f}\tASPRY  {:.3f}", AspectRatio.x, AspectRatio.y);
		const char* status = isFixedAspectRatio ? "True" : "False";
		ENGINE_TRACE("Stat {}", status);
#endif

		m_Bounds.RecalculateBound(m_Zoom, isFixedAspectRatio);
		RecalculateProjectionMatrix();
		RecalculateProjectionViewMatrix();
		return true;
	}

}