#pragma once

#include "Core/Kareeem.h"

namespace Karem {

	class OrthographicCamera 
	{
	public:
		OrthographicCamera() = default;
		OrthographicCamera(float left, float right, float bottom, float top) 
		{
			m_Projection = glm::ortho<float>(left, right, bottom, top);
		}

		glm::mat4& GetProjectionMatrix() { return m_Projection; }
		glm::mat4& GetViewMatrix() { return m_View; }

	private:
		glm::mat4 m_Projection;
		glm::mat4 m_View;
	};

}
