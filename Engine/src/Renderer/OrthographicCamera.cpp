#include "Core/Kareeem.h"
#include "OrthographicCamera.h"

#include "Event/KeyCode.h"
#include "Event/Input.h"

#define CAMERA_DEBUG 0

namespace Karem {

	void OrthographicCamera::RecalculateProjectionMatrix(float aspectRatio)
	{
		float right = (m_Size * aspectRatio) / 2;
		float left = -right;
		float top = m_Size / 2;
		float bottom = -top;

		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, m_Near, m_Far);
	}

}