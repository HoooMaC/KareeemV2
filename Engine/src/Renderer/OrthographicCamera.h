#pragma once

#include "Core/Kareeem.h"
#include "Core/TimeStep.h"

#include "Event/Event.h"
#include "Event/MouseEvent.h"

namespace Karem {

	class OrthographicCamera 
	{
	public:
		OrthographicCamera() = default;
		OrthographicCamera(float aspectRatio, bool rotation = true) 
			: m_AspectRatio(aspectRatio)
		{
			m_ProjectionMatrix = glm::ortho(-m_AspectRatio, m_AspectRatio, -m_Zoom, m_Zoom, -1.0f, 1.0f);
			m_ViewMatrix = glm::mat4(1.0f);

			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
		 
		glm::vec3& GetPosition() { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateMatrix(); }

		float& GetRotation() { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateMatrix(); }

		float& GetZoom() { return m_Zoom; }
		void SetZoom(float zoom) { m_Zoom = zoom; RecalculateMatrix(); }

	public:
		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);

	private:
		void RecalculateMatrix()
		{
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
				glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

			m_ViewMatrix = glm::inverse(transform);
			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}
	private:
		bool MouseScrolledEventAction(MouseScrolledEvent& event);
	private:
		float m_AspectRatio;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = glm::vec3(0.0f);
		float m_Rotation = 0.0f;
		float m_Zoom = 1.0f;
	};

}
