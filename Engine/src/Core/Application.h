#pragma once

#include "Core/Window.h"
#include "Layer/Layers.h"

#include "Event/Event.h"
#include "Event/AppEvent.h"

namespace Karem {
	
	enum class AppType
	{
		None = 0,
		GameEngine
	};

	// note the implemetation of the startup function still ini KaremEditor.cpp
	void StartUpApplication(AppType type);
	void RunApplication();
	void ShutdownApplication();

	class Application
	{
	public:
		Application() = default;
		Application(const WindowProperty& props);
		virtual ~Application();

		virtual void Run();
		virtual void EventHandler(Event& event);
		virtual void* GetNativeWindow() { return m_Window.GetNativeWindow(); }
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

	extern constinit Application* s_Application;
	inline Application* GetApplication() { return s_Application; }

}
