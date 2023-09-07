#include "Sandbox.h"

//#include "TriangleLayer.h"

#include <glad/glad.h>

// TEMP : THIS SHOULD NOT BE HERE (MAYBE)
#include "imgui.h"
#include "imgui_setup.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

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
	float middleX = 0, middleY = 0;
	float size = 1;

	glm::vec4 color = Karem::HexToVec4("#0099DD");

	float squareVertices[4 * 9] =
	{
		   middleX - (size / 2),  middleY - (size / 2), 0.0f, color.r, color.g, color.b, color.a, 0.0f, 0.0f, // kiri bawah
		   middleX + (size / 2),  middleY - (size / 2), 0.0f, color.r, color.g, color.b, color.a, 1.0f, 0.0f, // kanan bawah
		   middleX + (size / 2),  middleY + (size / 2), 0.0f, color.r, color.g, color.b, color.a, 1.0f, 1.0f, // kanan atas
		   middleX - (size / 2),  middleY + (size / 2), 0.0f, color.r, color.g, color.b, color.a, 0.0f, 1.0f  // kiri atas
	};

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

	m_Camera = Karem::OrthographicCamera((float)1920 / 1080);
	m_Shader = Karem::CreateShader("res\\shader\\position_color_vertex.glsl", "res\\shader\\position_color_fragment.glsl");
	m_VertexArray = Karem::CreateVertexArray();
	m_TextureBasic = Karem::CreateTexture2D("res\\texture\\diamond_sword.png");
	m_TextureSlot = m_TextureBasic->GetSlot();

	Karem::Renderer::SetShader(m_Shader);
	Karem::Renderer::SetVertexArray(m_VertexArray);
	Karem::Renderer::UpdateUniform("uTexture", (void*)&m_TextureSlot);

	// mungkin disini harusnya nggak ada, apalagi kalau batch rendering
	std::shared_ptr<Karem::VertexBuffer> squareVertexBuffer = Karem::CreateVertexBuffer((void*)squareVertices, sizeof(squareVertices));
	std::shared_ptr<Karem::IndexBuffer> squareIndexBuffer = Karem::CreateIndexBuffer((void*)squareIndices, 6);

	Karem::Renderer::AddVertexBuffer(squareVertexBuffer);
	Karem::Renderer::SetIndexBuffer(squareIndexBuffer);

	//maybe this can be automated by glGetActiveAttrib
	Karem::BufferLayout layout = {
		{ Karem::ShaderDataType::Vec3, "aPos" },
		{ Karem::ShaderDataType::Vec4, "aColor" },
		{ Karem::ShaderDataType::Vec2, "aTexCoord" }
	};
	squareVertexBuffer->SetLayout(layout);

	squareVertexBuffer->ApplyLayout();
}

void Sandbox::Run()
{
	while (m_Running)
	{
		float time = glfwGetTime();
		Karem::TimeStep timeStep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		m_Camera.OnUpdate(timeStep);

		Karem::RendererCommand::Clear();
		Karem::RendererCommand::ClearColor("#3F1D38");

		Karem::Renderer::BeginScene(m_Camera);

		imgui::BeginFrame();

		//for (std::shared_ptr<Karem::Layer>& layer : m_Layers)
		//{
		//	if (layer->GetStatus())
		//		layer->OnUpdate(timeStep);
		//}

		Karem::Renderer::Draw();

		imgui::EndFrame();
		Karem::Renderer::EndScene();
			
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
