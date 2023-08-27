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
		
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void* userParam) {
			// Output informasi error ke konsol
			ENGINE_ERROR("OpenGL ERROR id [{}]: \n{}", id, message);
			__debugbreak();
		}, nullptr);

	}

	void OpenGLContext::SwapBuffers(GLFWwindow* window)
	{
		glfwSwapBuffers(window);
	}

}
