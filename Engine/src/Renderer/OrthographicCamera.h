#pragma once

#include "Core/Kareeem.h"
#include "Core/TimeStep.h"

#include "Event/Event.h"
#include "Event/MouseEvent.h"
#include "Event/AppEvent.h"

namespace Karem {

	class OrthographicCamera 
	{
		struct OrthographicCameraAtrribute
		{
			float Right, Left;
			float Top, Bottom;
			float NearClip, FarClip;
			glm::vec2 AspectRatio;
			OrthographicCameraAtrribute() = default;
			OrthographicCameraAtrribute(const glm::vec2& aspectRatio, float zoomPercentage, float aspectRatioStatus)
				: AspectRatio(aspectRatio)
			{
				RecalculateBound(zoomPercentage, aspectRatioStatus);
			}

			void RecalculateBound(float zoomPercentage, bool cameraAspectRatioStatus)
			{
				if (cameraAspectRatioStatus)
				{
					float factor = GetBaseX(AspectRatio) / (AspectRatio.x * 100);
					float distanceToObject = std::max(std::abs(Right), std::abs(Left));
					Right = (AspectRatio.x * factor * zoomPercentage) / 2;
					Left = -Right;
					Top = (AspectRatio.y * factor * zoomPercentage) / 2;
					Bottom = -Top;
					// this is temporary : TODO : thinking about the value of Near and Far Clip
					NearClip = -1;
					FarClip = 1;
				}
				else
				{
					float aspectRatio = AspectRatio.x / AspectRatio.y;
					Right = (aspectRatio * zoomPercentage) / 2;
					Left = -Right;
					Top = zoomPercentage / 2;
					Bottom = -Top;
					// this is temporary : TODO : thinking about the value of Near and Far Clip
					NearClip = -1;
					FarClip = 1;
				}
			}
		private:
			static float GetBaseX(const glm::vec2& aspectRatio)
			{
				if (aspectRatio.x == 16 and aspectRatio.y == 9)
					return 1920;
				else if (aspectRatio.x == 21 and aspectRatio.y == 9)
					return 3440;
				else if (aspectRatio.x == 16 and aspectRatio.y == 10)
					return 1920;
				else if (aspectRatio.x == 4 and aspectRatio.y == 5)
					return 1920;
				else if (aspectRatio.x == 4 and aspectRatio.y == 3)
					return 1024;
				else if (aspectRatio.x == 9 and aspectRatio.y == 16)
					return 1080;

				ENGINE_ASSERT(false, "Unsupported Aspect Ratio");
				return 0;
			}

			static float GetBaseY(const glm::vec2& aspectRatio)
			{
				if (aspectRatio.x == 16 and aspectRatio.y == 9)
					return 1080;
				else if (aspectRatio.x == 21 and aspectRatio.y == 9)
					return 1440;
				else if (aspectRatio.x == 16 and aspectRatio.y == 10)
					return 1200;
				else if (aspectRatio.x == 4 and aspectRatio.y == 5)
					return 1250;
				else if (aspectRatio.x == 4 and aspectRatio.y == 3)
					return 768;
				else if (aspectRatio.x == 9 and aspectRatio.y == 16)
					return 1920;
				ENGINE_ASSERT(false, "Unsupported Aspect Ratio");
				return 0;
			}
		};

	public:
		OrthographicCamera() = default;
		OrthographicCamera(const glm::vec2& aspectRatio, float zoomPercentage) 
			: m_AspectRatio(aspectRatio), m_Zoom(zoomPercentage)
		{
			m_Bounds = OrthographicCameraAtrribute(aspectRatio, m_Zoom, isFixedAspectRatio);
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

		void SetAspectRatioStatus(bool status) { isFixedAspectRatio = status; }
		glm::vec2 GetAspectRatio() { return m_Bounds.AspectRatio; }

		void SetAspectRatio(const glm::vec2 aspectRatio)
		{
			if (isFixedAspectRatio)
			{
				ENGINE_WARN("The current aspect ratio is fixed cannot be change");
				return;
			}
			m_Bounds.AspectRatio = aspectRatio;
		}

		float& GetZoom() 
		{ 
			return m_Zoom; 
			RecalculateProjectionMatrix();
		}

		void SetZoom(float zoom) 
		{ 
			m_Zoom = zoom;
			m_Bounds.RecalculateBound(m_Zoom, isFixedAspectRatio);
			RecalculateProjectionMatrix();
			RecalculateProjectionViewMatrix();
		}

		const OrthographicCameraAtrribute& GetBounds() const { return m_Bounds; }

	public:
		void Update(TimeStep ts);
		void EventHandler(Event& e);

	private:
		void RecalculateProjectionMatrix()
		{
			m_ProjectionMatrix = glm::ortho(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top, m_Bounds.NearClip, m_Bounds.FarClip);
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
		bool WindowResizeEventAction(WindowResizeEvent& event);
	private:
		glm::vec2 m_AspectRatio = { 16, 9 };
		float m_Zoom = 10.0f;
		bool isFixedAspectRatio = false;
		OrthographicCameraAtrribute m_Bounds;

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = glm::vec3(0.0f);
		float m_Rotation = 0.0f;
	};

}
