#pragma once

#include "Core/Window.h"

#include "Event/Event.h"
#include "Event/AppEvent.h"
namespace Karem {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void Run();

	private:
		void Init();
		void Shutdown();

		void EventHandler(Event& event);

	private:
		bool WindowCloseAction(WindowCloseEvent& event);
	private:
		Window m_Window;
		bool m_Running = true;
	};

	Application* CreateApplication();

}
