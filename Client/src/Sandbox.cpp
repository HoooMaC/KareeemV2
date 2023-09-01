#include "Sandbox.h"

#include <glad/glad.h>

// TEMP : THIS SHOULD NOT BE HERE (MAYBE)
#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

Karem::Application* Karem::CreateApplication()
{
	return new Sandbox();
}

Sandbox::Sandbox()
{
	Init();

	m_Camera = Karem::OrthographicCamera(-16.0f, 16.0f, -9.0f, 9.0f);

	std::shared_ptr<Karem::Shader> shader = Karem::CreateShader("res\\shader\\position_color_vertex.glsl", "res\\shader\\position_color_fragment.glsl");
	std::shared_ptr<Karem::VertexArray> vertexArray = Karem::CreateVertexArray();

	Karem::Renderer::SetShader(shader);
	Karem::Renderer::SetVertexArray(vertexArray);

	PushLayer(std::make_shared<AppLayer>());
	//PushLayer(std::make_shared<TestLayer>("just for testing"));
}

Sandbox::~Sandbox()
{
	Shutdown();
}

void Sandbox::Run()
{
	while (m_Running)
	{
		float time = glfwGetTime();
		Karem::TimeStep timeStep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		Karem::RendererCommand::Clear();
		Karem::RendererCommand::ClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });

		if (Karem::Input::IsKeyPressed(KAREM_KEY_E))
		{
			float& rotation = m_Camera.GetRotation();
			rotation += 180.0f * timeStep;
			m_Camera.SetRotation(rotation);
		}
		else if (Karem::Input::IsKeyPressed(KAREM_KEY_Q))
		{
			float& rotation = m_Camera.GetRotation();
			rotation -= 180.0f * timeStep;
			m_Camera.SetRotation(rotation);
		}

		Karem::Renderer::BeginScene(m_Camera);

		{
			Karem::ImGUILayer::BeginScene();

			ImGui::Begin("Layer Control");

			for (int i = 0; i < m_Layers.GetSize(); i++)
			{
				std::shared_ptr<Karem::Layer>& layer = m_Layers.GetLayerAt(i);
				std::pair<std::string&, bool&> layerData = layer->GetLayerData();
				ImGui::Checkbox(layerData.first.c_str(), &layerData.second);

				if (i > 0)
				{
					ImGui::SameLine();
					std::string buttonName = "Make up " + layerData.first;
					if (ImGui::Button(buttonName.c_str()))
					{
						std::swap(m_Layers.GetLayerAt(i), m_Layers.GetLayerAt(i - 1));
					}
				}

				if (i < m_Layers.GetSize() - 1)
				{
					ImGui::SameLine();
					std::string buttonName = "Make down " + layerData.first;
					if (ImGui::Button(buttonName.c_str()))
					{
						std::swap(m_Layers.GetLayerAt(i), m_Layers.GetLayerAt(i + 1));
					}
				}
			}

			// untuk ImGUI, ada loopingnya tersendiri
			// berarti ada list atau stacknya sendiri
			ImGui::End();

			Karem::ImGUILayer::EndScene();
		}

		for (std::shared_ptr<Karem::Layer>& layer : m_Layers)
		{
			if (layer->GetStatus())
				layer->OnUpdate(timeStep); // Memanggil fungsi yang diinginkan dari shared_ptr
		}

		Karem::Renderer::EndScene();

		Karem::Renderer::Draw();

		m_Window.OnUpdate();
	}
}

void Sandbox::Init()
{
	m_Window.SetEventCallbacks(std::bind(&Application::EventHandler, this, std::placeholders::_1));

	// ImGUI init
	GLFWwindow* window = Karem::GraphicsContext::GetContextCurrent();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 450");

	io.Fonts->AddFontFromFileTTF("res/font/Helvetica.ttf", 14.f);
}

void Sandbox::Shutdown()
{
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Sandbox::EventHandler(Karem::Event& event)
{
	Karem::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Karem::WindowCloseEvent>(std::bind(&Sandbox::WindowCloseAction, this, std::placeholders::_1));
	dispatcher.Dispatch<Karem::KeyPressedEvent>(std::bind(&Sandbox::KeyPressedAction, this, std::placeholders::_1));

	for (auto it = m_Layers.rbegin(); it != m_Layers.rend(); ++it)
	{
		if((*it)->GetStatus())
			(*it)->EventHandler(event); // Memanggil fungsi yang diinginkan dari shared_ptr
	}

	//ENGINE_TRACE(event);
}

bool Sandbox::WindowCloseAction(Karem::WindowCloseEvent& event)
{
	m_Running = false;
	return true;
}

bool Sandbox::KeyPressedAction(Karem::KeyPressedEvent& event)
{
	//float& rotation = m_Camera.GetRotation();
	//if (event.GetKeyCode() == KAREM_KEY_E)
	//{
	//	rotation += 10.0f;
	//	m_Camera.SetRotation(rotation);
	//}
	//else if (event.GetKeyCode() == KAREM_KEY_Q)
	//{
	//	rotation -= 10.0f;
	//	m_Camera.SetRotation(rotation);
	//}
	return true;
}

void Sandbox::PushLayer(std::shared_ptr<Karem::Layer> layer)
{
	m_Layers.PushLayer(layer);
}

void Sandbox::PushOverlay(std::shared_ptr<Karem::Layer> overlay)
{
	m_Layers.PushOverlay(overlay);
}

void Sandbox::PopLayer(std::shared_ptr<Karem::Layer> layer)
{
	m_Layers.PopLayer(layer);
}

void Sandbox::PopOverlay(std::shared_ptr<Karem::Layer> overlay)
{
	m_Layers.PopOverlay(overlay);
}
