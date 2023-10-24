#include "Core/Kareeem.h"
#include "GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Karem {

	GLFWwindow* GraphicsContext::GetContextCurrent()
	{
		return glfwGetCurrentContext();
	}

	void GraphicsContext::SetCurrentContext(GLFWwindow* context)
	{
		ENGINE_ASSERT(context, "INVALID CONTEXT");
		return glfwMakeContextCurrent(context);
	}

}