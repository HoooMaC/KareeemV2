#include "Core/Kareeem.h"

#include "Input.h"


#include "Core/Renderer/GraphicsContext.h"

//THIS IS STILL SPECIFIC ON WINDOWs
#include <GLFW/glfw3.h>

namespace Karem {

	bool Input::IsKeyPressed(int keycode)
	{
		GLFWwindow* window = GraphicsContext::GetContextCurrent();
		int state = glfwGetKey(window, keycode);
		return state == KAREM_PRESS || state == KAREM_REPEAT;
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		GLFWwindow* window = GraphicsContext::GetContextCurrent();
		int state = glfwGetMouseButton(window, button);
		return state == KAREM_PRESS;
	}

	float Input::GetMousePositionX()
	{
		double xpos;
		GLFWwindow* window = GraphicsContext::GetContextCurrent();
		glfwGetCursorPos(window, &xpos, nullptr);
		return (float)xpos;
	}

	float Input::GetMousePositionY()
	{
		double ypos;
		GLFWwindow* window = GraphicsContext::GetContextCurrent();
		glfwGetCursorPos(window, nullptr, &ypos);
		return (float)ypos;
	}

}