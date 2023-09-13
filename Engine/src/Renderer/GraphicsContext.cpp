#include "Core/Kareeem.h"
#include "GraphicsContext.h"

#include <GLFW/glfw3.h>

GLFWwindow* Karem::GraphicsContext::GetContextCurrent()
{
	return glfwGetCurrentContext();
}
