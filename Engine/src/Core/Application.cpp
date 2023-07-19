#include "Kareeem.h"
#include "Application.h"

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
		while (m_Running)
		{
			std::cout << "The Application is Running in Engine \n";
		}
	}

	void Application::Shutdown()
	{
	}

}