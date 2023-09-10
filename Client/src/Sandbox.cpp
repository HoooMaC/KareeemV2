#include "Sandbox.h"

//#include "TriangleLayer.h"

//#include <glad/glad.h>

// TEMP : THIS SHOULD NOT BE HERE (MAYBE)
//#include "imgui.h"
//#include "imgui_setup.h"
//#include "backends/imgui_impl_glfw.h"
//#include "backends/imgui_impl_opengl3.h"

//#include <GLFW/glfw3.h>

Karem::Application* Karem::CreateApplication()
{
	return new Sandbox();
}

Sandbox::Sandbox()
	: Application({"Sandbox Application", 1920, 1080})
{
	Init();
}

void Sandbox::Init()
{
	m_Camera = Karem::OrthographicCamera((float)1920 / 1080);
}

void Sandbox::Run()
{
	while (m_Running)
	{
		float time = Karem::Platform::GetTime();
		Karem::TimeStep timeStep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		m_Camera.OnUpdate(timeStep);

		Karem::RendererCommand::Clear();
		Karem::RendererCommand::ClearColor("#3F1D38");

		Karem::Renderer2D::BeginScene();

		Karem::Renderer2D::EndScene();

		//imgui::BeginFrame();

		//for (std::shared_ptr<Karem::Layer>& layer : m_Layers)
		//{
		//	if (layer->GetStatus())
		//		layer->OnUpdate(timeStep);
		//}

		//imgui::EndFrame();
		m_Window.OnUpdate();
	}
}

Sandbox::~Sandbox()
{
	Shutdown();
}

void Sandbox::Shutdown()
{
}

void Sandbox::EventHandler(Karem::Event& event)
{
	Karem::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Karem::WindowCloseEvent>(std::bind(&Sandbox::WindowCloseAction, this, std::placeholders::_1));

	m_Camera.OnEvent(event);
	//for (auto it = m_Layers.rbegin(); it != m_Layers.rend(); ++it)
	//{
	//	if((*it)->GetStatus())
	//		(*it)->EventHandler(event);
	//}

	//ENGINE_TRACE(event);
}

bool Sandbox::WindowCloseAction(Karem::WindowCloseEvent& event)
{
	m_Running = false;
	return true;
}

//void Sandbox::PushLayer(std::shared_ptr<Karem::Layer> layer)
//{
//	m_Layers.PushLayer(layer);
//}
//
//void Sandbox::PushOverlay(std::shared_ptr<Karem::Layer> overlay)
//{
//	m_Layers.PushOverlay(overlay);
//}
//
//void Sandbox::PopLayer(std::shared_ptr<Karem::Layer> layer)
//{
//	m_Layers.PopLayer(layer);
//}
//
//void Sandbox::PopOverlay(std::shared_ptr<Karem::Layer> overlay)
//{
//	m_Layers.PopOverlay(overlay);
//}
