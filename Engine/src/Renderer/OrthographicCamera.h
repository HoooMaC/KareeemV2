#pragma once

#include "Core/Kareeem.h"
#include "Core/TimeStep.h"

#include "Event/Event.h"
#include "Event/MouseEvent.h"

namespace Karem {

	class OrthographicCamera 
	{
		struct CameraAtrribute
		{
			float Right, Left;
			float Top, Bottom;
			glm::vec2 AspectRatio;
			CameraAtrribute() = default;
			CameraAtrribute(const glm::vec2& aspectRatio, float zoomPercentage)
				: AspectRatio(aspectRatio)
			{
				RecalculateBound(zoomPercentage);
			}

			void RecalculateBound(float zoomPercentage)
			{
				// aspect Ratio x = 16 zoom percentage = 100
				float Base = GetBaseX(AspectRatio);
				float factor = Base / (AspectRatio.x * 100);
				// expected factor = 1.2


				Right = (AspectRatio.x * factor * zoomPercentage) / 2;
				// expected Right = 960
				Left = -Right;
				// expected Left = -960

				Top = (AspectRatio.y * factor * zoomPercentage) / 2;
				// expected top = 540
				Bottom = -Top;
				//expected top = -540

			}
			static float GetBaseX(const glm::vec2& aspectRatio)
			{
				if (aspectRatio.x == 16 and aspectRatio.y == 9)
				{
					return 1920;
				}
				else if (aspectRatio.x == 21 and aspectRatio.y == 9)
				{
					return 3440;
				}
				else if (aspectRatio.x == 16 and aspectRatio.y == 10)
				{
					return 1920;
				}
				else if (aspectRatio.x == 4 and aspectRatio.y == 5)
				{
					return 1920;
				}
				else if (aspectRatio.x == 4 and aspectRatio.y == 3)
				{
					return 1024;
				}
				else if (aspectRatio.x == 9 and aspectRatio.y == 16)
				{
					return 1080;
				}
				else
				{
					ENGINE_ASSERT(false, "Unsupported Aspect Ration");
				}
			}
			static float GetBaseY(const glm::vec2& aspectRatio)
			{
				if (aspectRatio.x == 16 and aspectRatio.y == 9)
				{
					return 1080;
				}
				else if (aspectRatio.x == 21 and aspectRatio.y == 9)
				{
					return 1440;
				}
				else if (aspectRatio.x == 16 and aspectRatio.y == 10)
				{
					return 1200;
				}
				else if (aspectRatio.x == 4 and aspectRatio.y == 5)
				{
					return 1250;
				}
				else if (aspectRatio.x == 4 and aspectRatio.y == 3)
				{
					return 768;
				}
				else if (aspectRatio.x == 9 and aspectRatio.y == 16)
				{
					return 1920;
				}
				else
				{
					ENGINE_ASSERT(false, "Unsupported Aspect Ration");
				}
			}
		};

	public:
		OrthographicCamera() = default;
		OrthographicCamera(const glm::vec2& aspectRatio, float zoomPercentage) 
			: m_AspectRatio(aspectRatio), m_Zoom(zoomPercentage)
		{
			m_Bounds = CameraAtrribute(aspectRatio, m_Zoom);
			m_ProjectionMatrix = glm::ortho(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
			m_ViewMatrix = glm::mat4(1.0f);

			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
		 
		glm::vec3& GetPosition() { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateProjectionViewMatrix(); }

		float& GetRotation() { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateProjectionViewMatrix(); }

		float& GetZoom() { return m_Zoom; RecalculateProjectionMatrix(); }
		void SetZoom(float zoom) { m_Zoom = zoom; m_Bounds.RecalculateBound(m_Zoom); RecalculateProjectionMatrix(); RecalculateProjectionViewMatrix(); }

		const CameraAtrribute& GetBounds() const { return m_Bounds; }
	public:
		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);

	private:
		void RecalculateProjectionMatrix()
		{
			m_ProjectionMatrix = glm::ortho(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
		}

		void RecalculateProjectionViewMatrix()
		{
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
				glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

			m_ViewMatrix = glm::inverse(transform);
			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}
	private:
		bool MouseScrolledEventAction(MouseScrolledEvent& event);
	private:
		glm::vec2 m_AspectRatio = { 16, 9 };
		float m_Zoom = 10.0f;
		CameraAtrribute m_Bounds;

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = glm::vec3(0.0f);
		float m_Rotation = 0.0f;
	};

}
