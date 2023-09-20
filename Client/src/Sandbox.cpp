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

	// this is overriding texture in index 3 in texture renderer
	m_Texture = Karem::CreateTexture2D("res/texture/spritesheet/city_tilemap.png", 1);
	m_SpriteSheet = Karem::CreateSubTexture(m_Texture, { 21,11 }, { 8,8 }, { 3,4 });

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
		Karem::RendererCommand::ClearColor("#026773");

		Karem::Renderer2D::BeginScene(m_Camera);

		Karem::Renderer2D::SubmitSubTexturedQuad({ 0.0f, 0.0f, 0.0f }, { 3.0f, 4.0f }, m_SpriteSheet, 1.0f);

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
	dispatcher.Dispatch<Karem::WindowResizeEvent>(std::bind(&Sandbox::WindowResizeAction, this, std::placeholders::_1));

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

bool Sandbox::WindowResizeAction(Karem::WindowResizeEvent& event)
{
	m_Window.Resize(event.GetWidth(), event.GetHeight());
	return true;
}
