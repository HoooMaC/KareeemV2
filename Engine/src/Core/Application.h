#pragma once

#include "Core/Window.h"

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
		Window m_Window;
		bool m_Running = true;
	};

	Application* CreateApplication();

}
