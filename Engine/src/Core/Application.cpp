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
	}

	void Application::Shutdown()
	{
	}

}