#pragma once

#include "Core/Kareeem.h"
#include "Core/TimeStep.h"

#include "Event/Event.h"
#include "Event/MouseEvent.h"
#include "Event/AppEvent.h"

namespace Karem {

	class CameraHandler;

	class OrthographicCamera 
	{
		// this is temporary
		friend class CameraHandler;
	public:
		OrthographicCamera(float aspectRatio = 16.0f/9.0f) { RecalculateProjectionMatrix(aspectRatio); }
		OrthographicCamera(float orthographicSize, float aspectRatio, float orthographicNear, float orthographicFar) 
			: m_Size(orthographicSize), m_Near(orthographicNear), m_Far(orthographicFar)
		{
			RecalculateProjectionMatrix(aspectRatio);
		}

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		 
		float GetOrthographicSize() { return m_Size; }
		void SetOrthographicSize(float size, float aspectRatio) { m_Size = size; RecalculateProjectionMatrix(aspectRatio); }

		float GetOrthographicNear() { return m_Near; }
		void SetOrthographicNear(float nearClip, float aspectRatio) { m_Near = nearClip; RecalculateProjectionMatrix(aspectRatio); }

		float GetOrthographicFar() { return m_Far; }
		void SetOrthographicFar(float farClip, float aspectRatio) { m_Far = farClip; RecalculateProjectionMatrix(aspectRatio); }

	private:
		void RecalculateProjectionMatrix(float aspectRatio);

	private:
		float m_Size = 5.0f;
		float m_Near = -5.0f, m_Far = 5.0f;

		glm::mat4 m_ProjectionMatrix;
	};

}
