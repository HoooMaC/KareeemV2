#include "Core/Kareeem.h"
#include "Env/Color.h"

#include "OpenGLRendererAPI.h"
#include <glad/glad.h>

namespace Karem {

	void OpenGLRendererAPI::Init()
	{
	}

	void OpenGLRendererAPI::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::ClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::ClearColor(Color color)
	{
		glm::vec4 resultColor = HexToVec4<glm::vec4>(color);
		glClearColor(resultColor.r, resultColor.g, resultColor.b, resultColor.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::Draw(uint32_t count)
	{
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

}