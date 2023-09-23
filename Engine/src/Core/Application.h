#pragma once

#include "Core/Window.h"
#include "Layer/Layers.h"

#include "Event/Event.h"
#include "Event/AppEvent.h"

#include "Renderer/OrthographicCamera.h"
#include "Renderer/FrameBuffer.h"

#include "imgui_setup.h"

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
		virtual void Init() = 0;
		virtual void Shutdown();

	protected:
		bool WindowCloseAction(WindowCloseEvent& event);
		bool WindowResizeAction(WindowResizeEvent& event);

	protected:
		Window m_Window;
		OrthographicCamera m_Camera;
		Layers m_Layers;
		std::shared_ptr<Karem::FrameBuffer> m_FrameBuffer;

		float m_LastFrameTime = 0.0f;

		bool m_Running = true;
	};

	Application* CreateApplication();

}
