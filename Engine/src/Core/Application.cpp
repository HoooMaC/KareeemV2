#include "Core/Kareeem.h"
#include "Application.h"

#include <Event/Event.h>
#include <Event/AppEvent.h>

#include <GLFW/glfw3.h>

namespace Karem {

	Application::Application()
	{
		Init();
	}

	void Application::Init()
	{
		m_Window.SetEventCallbacks(std::bind(&Application::EventHandler, this, std::placeholders::_1));
	}

	Application::~Application()
	{
		Shutdown();
	}

	void Application::Shutdown()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Window.OnUpdate();

			for (std::shared_ptr<Layer>& layer : m_Layers)
			{
				layer->OnUpdate(); // Memanggil fungsi yang diinginkan dari shared_ptr
			}

		}
	}

	void Application::EventHandler(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::WindowCloseAction, this, std::placeholders::_1));

		for (auto it = m_Layers.rbegin(); it != m_Layers.rend(); ++it)
		{
			(*it)->EventHandler(event); // Memanggil fungsi yang diinginkan dari shared_ptr
		}

		ENGINE_TRACE(event);
	}

	bool Application::WindowCloseAction(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}
}