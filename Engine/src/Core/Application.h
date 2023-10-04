#pragma once

#include "Core/Window.h"
#include "Layer/Layers.h"

#include "Event/Event.h"
#include "Event/AppEvent.h"

namespace Karem {
	

	class Application
	{
	public:
		Application() = default;
		Application(const WindowProperty& props);
		virtual ~Application();

		virtual void Run();
		virtual void EventHandler(Event& event);

	protected:
		virtual void Init() {}
		virtual void Shutdown() {}

	protected:
		bool WindowCloseAction(WindowCloseEvent& event);

	protected:
		Window m_Window;
		Layers m_Layers;

		float m_LastFrameTime = 0.0f;

		bool m_Running = true;
	};

	extern Application* CreateApplication();
}
