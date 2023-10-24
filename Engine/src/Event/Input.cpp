#include "Core/Kareeem.h"

#include "Input.h"


#include "Renderer/GraphicsContext.h"

// TEMPORARY!!! THIS IS STILL SPECIFIC ON WINDOWS
#include <GLFW/glfw3.h>

namespace Karem {

	bool Input::IsKeyPressed(int keycode)
	{
		GLFWwindow* window = GraphicsContext::GetContextCurrent();
		int state = glfwGetKey(window, keycode);
		return state == (int16_t)Key::Press || state == (int16_t)Key::Repeat;
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		GLFWwindow* window = GraphicsContext::GetContextCurrent();
		int state = glfwGetMouseButton(window, button);
		return state == (uint16_t)Key::Press;
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