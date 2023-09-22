#include "Core/Kareeem.h"

#include "Application.h"

#include "Renderer/Renderer2D.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/OrthographicCamera.h"

#include "Platform/Windows/Platform.h"

namespace Karem {

	static constexpr int32_t appWidth = 1280, appHeight = 720;

	Application::Application(const WindowProperty& props) : m_Window(props)
	{
		m_Window.SetEventCallbacks(std::bind(&Application::EventHandler, this, std::placeholders::_1));
		m_Camera = OrthographicCamera((float)appWidth / appHeight);

		GLFWwindow* window = Karem::GraphicsContext::GetContextCurrent();
		imgui::InitializeImGUI(window);

		Renderer2D::Initialize();
	}

	Application::~Application()
	{
		imgui::Shutdown();
		Renderer2D::Shutdown();

		Shutdown();
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float time = Platform::GetTime();
			TimeStep timeStep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			m_Camera.OnUpdate(timeStep);

			RendererCommand::Clear();
			RendererCommand::ClearColor("#026773");

			//imgui::BeginFrame();

			Renderer2D::BeginScene(m_Camera);

			for (std::shared_ptr<Layer>& layer : m_Layers)
			{
				if (layer->GetStatus())
					layer->OnUpdate(timeStep);
			}

			Renderer2D::EndScene();

			//imgui::EndFrame();
			m_Window.OnUpdate();
		}

	}

	void Application::EventHandler(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::WindowCloseAction, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&Application::WindowResizeAction, this, std::placeholders::_1));

		m_Camera.OnEvent(event);
		//for (auto it = m_Layers.rbegin(); it != m_Layers.rend(); ++it)
		//{
		//	if((*it)->GetStatus())
		//		(*it)->EventHandler(event);
		//}

		//ENGINE_TRACE(event);
	}

	void Application::Shutdown()
	{
	}

	bool Application::WindowCloseAction(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}

	bool Application::WindowResizeAction(WindowResizeEvent& event)
	{
		m_Window.Resize(event.GetWidth(), event.GetHeight());
		return true;
	}
}