#include "Core/Kareeem.h"
#include "Application.h"

#include <GLFW/glfw3.h>

namespace Karem {

	Application::Application()
	{
		Init();
	}

	Application::~Application()
	{
		Shutdown();
	}

	void Application::Init()
	{
	}

	void Application::Run()
	{
		GLFWwindow* window = glfwGetCurrentContext();

		while (glfwWindowShouldClose(window));
		{
			//glClear(GL_COLOR_BUFFER_BIT);
			//glClearColor(0.8f, 0.4f, 0.3f, 1.0f);
			glfwPollEvents();
			std::cout << "The Application is Running in Engine \n";
		}
	}

	void Application::Shutdown()
	{
	}

}