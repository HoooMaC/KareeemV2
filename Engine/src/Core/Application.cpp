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
		m_Window.SetEventCallbacks(std::bind(&Application::EventHandler, this, std::placeholders::_1));
	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Window.OnUpdate();
		}
	}

	void Application::Shutdown()
	{
	}

	void Application::EventHandler(Event& event)
	{
	}

}