#include "Core/Kareeem.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Karem {

	void OpenGLContext::Init()
	{
		int initGlad = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		// TODO : Need some assertion here if the initialization is failed

		ENGINE_INFO("OpenGL Specification");
		ENGINE_INFO("OpenGL Renderer : {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		ENGINE_INFO("OpenGL Renderer : {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		ENGINE_INFO("OpenGL Version : {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	}

	void OpenGLContext::SwapBuffers(GLFWwindow* window)
	{
		glfwSwapBuffers(window);
	}

}
