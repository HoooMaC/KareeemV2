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
			RecalculateMatrix();
		}
		else if (Karem::Input::IsKeyPressed(KAREM_KEY_E))
		{
			m_Rotation += 180.0f * ts;
			RecalculateMatrix();
		}
		else if (Karem::Input::IsKeyPressed(KAREM_KEY_Q))
		{
			m_Rotation -= 180.0f * ts;
			RecalculateMatrix();
		}

		if (Karem::Input::IsKeyPressed(KAREM_KEY_D))
		{
			m_Position.x += 10.0f * ts;
			RecalculateMatrix();
		}
		else if (Karem::Input::IsKeyPressed(KAREM_KEY_A))
		{
			m_Position.x -= 10.0f * ts;
			RecalculateMatrix();
		}

		if (Karem::Input::IsKeyPressed(KAREM_KEY_W))
		{
			m_Position.y += 10.0f * ts;
			RecalculateMatrix();
		}
		else if (Karem::Input::IsKeyPressed(KAREM_KEY_S))
		{
			m_Position.y -= 10.0f * ts;
			RecalculateMatrix();
		}
	}

	void OrthographicCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&OrthographicCamera::MouseScrolledEventAction, this, std::placeholders::_1));
	}

	bool OrthographicCamera::MouseScrolledEventAction(MouseScrolledEvent& event)
	{
		m_Zoom -= event.GetOffsetY() * 0.25;
		m_Zoom = std::max(m_Zoom, 0.25f);
		m_Zoom = std::min(m_Zoom, 10.0f);
		ENGINE_TRACE("{}", m_Zoom);
		m_ProjectionMatrix = glm::ortho(-m_AspectRatio * m_Zoom, m_AspectRatio * m_Zoom, -m_Zoom, m_Zoom, -1.0f, 1.0f);
		RecalculateMatrix();
		return true;
	}

}