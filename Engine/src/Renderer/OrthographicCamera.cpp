#include "Core/Kareeem.h"
#include "OrthographicCamera.h"

#include "Event/KeyCode.h"
#include "Event/Input.h"


namespace Karem {

	void OrthographicCamera::OnUpdate(TimeStep ts)
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

	void OrthographicCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&OrthographicCamera::MouseScrolledEventAction, this, std::placeholders::_1));
	}

	bool OrthographicCamera::MouseScrolledEventAction(MouseScrolledEvent& event)
	{
		float eventScrollData = event.GetOffsetY();;
		m_Zoom -= eventScrollData;
		//m_Zoom = std::max(m_Zoom, 100.0f);
		//m_Zoom = std::min(m_Zoom, 1.0f);

		if (m_Zoom > 100)
			m_Zoom = 100;
		else if (m_Zoom < 1)
			m_Zoom = 1;

		auto [Right, Left, Top, Bottom, AspectRatio] = GetBounds();

		ENGINE_DEBUG("{}", m_Zoom);
		ENGINE_TRACE("left {:.3f}\tright  {:.3f}", Left, Right);
		ENGINE_TRACE("Top  {:.3f}\tBottom {:.3f}", Top, Bottom);

		m_Bounds.RecalculateBound(m_Zoom);
		RecalculateProjectionMatrix();
		RecalculateProjectionViewMatrix();
		return true;
	}

}