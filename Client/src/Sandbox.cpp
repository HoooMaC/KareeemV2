#include "Sandbox.h"

//#include "TriangleLayer.h"

//#include <glad/glad.h>

// TEMP : THIS SHOULD NOT BE HERE (MAYBE)
//#include "imgui.h"
//#include "imgui_setup.h"
//#include "backends/imgui_impl_glfw.h"
//#include "backends/imgui_impl_opengl3.h"

Karem::Application* Karem::CreateApplication()
{
	return new Sandbox();
}

static constexpr int32_t appWidth = 1280, appHeight = 720;

Sandbox::Sandbox()
	: Application({ "Sandbox Application", appWidth, appHeight})
{
	Init();
}

void Sandbox::Init()
{
	m_Camera = Karem::OrthographicCamera((float)appWidth / appHeight);
}

void Sandbox::Run()
{
	while (m_Running)
	{
		float time = Karem::Platform::GetTime();
		Karem::TimeStep timeStep = time - m_LastFrameTime;
		//ENGINE_DEBUG("Frame time {}", timeStep);
		m_LastFrameTime = time;

		m_Camera.OnUpdate(timeStep);

		Karem::RendererCommand::Clear();
		Karem::RendererCommand::ClearColor("#3F1D38");

		Karem::Renderer2D::BeginScene(m_Camera);

		constexpr int32_t countQuad = 10;

		for (int y = 0; y < countQuad; y++)
		{
			for (int x = 0; x < countQuad; x++)
			{
				Karem::Renderer2D::SubmitQuad({ float(x), float(y), -0.1f}, glm::vec2(0.8f), {0.5f, 0.0f, 0.8f, 1.0f}, 0.0f);
			}
		}
		Karem::Renderer2D::SubmitTexturedQuad(glm::vec3(0.0f), glm::vec2(1.0f), 2.0f);
		Karem::Renderer2D::SubmitTriangle({ 0.0f, 0.0f, 0.1f }, glm::vec2(0.5f), { 0.0f, 0.5f, 0.8f, 1.0f }, 0.0f);

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