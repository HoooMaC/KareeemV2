#include "Sandbox.h"

#include <glad/glad.h>



// TEMP : THIS SHOULD NOT BE HERE (MAYBE)
#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

Karem::Application* Karem::CreateApplication()
{
	return new Sandbox();
}

void AppLayer::OnAttach()
{
	float triangleVertices[3 * 3] =
	{
		-0.5f, -0.5f, 0.5f,
		 0.5f, -0.5f, 0.5f,
		 0.0f,  0.5f, 0.5f
	};

	unsigned int triangleIndices[3] = { 0, 1, 2 };

	m_ShaderTriangle = Karem::Shader::CreateShader("res\\shader\\basic_vertex_shader.glsl", "res\\shader\\basic_fragment_shader.glsl");
	//std::dynamic_pointer_cast<Karem::OpenGLShader>(m_ShaderTriangle)->UpdateUniform("uMul", (void*)data);
	m_VertexArrayTriangle = Karem::VertexArray::CreateVertexArray();

	std::shared_ptr<Karem::VertexBuffer> vertexBufferBasic = Karem::VertexBuffer::CreateVertexBuffer((void*)triangleVertices, sizeof(triangleVertices));
	std::shared_ptr<Karem::IndexBuffer> indexBufferBasic = Karem::IndexBuffer::CreateIndexBuffer((void*)triangleIndices, 3);
	m_VertexArrayTriangle->AddVertexBuffer(vertexBufferBasic);
	m_VertexArrayTriangle->SetIndexBuffer(indexBufferBasic);

	{
		Karem::BufferLayout layout = {
			{ Karem::ShaderDataType::Vec3, "aPos" }
		};
		vertexBufferBasic->SetLayout(layout);
	}
	
	vertexBufferBasic->ApplyLayout();

	m_VertexArrayTriangle->UnBind();

	m_ShaderSquare = Karem::Shader::CreateShader("res\\shader\\position_color_vertex.glsl", "res\\shader\\position_color_fragment.glsl");
	
	float squareVertices[4 * 7] =
	{
		-0.7f, -0.7f, 0.6f, 0.2f, 0.0f, 0.8f, 1.0f, // kiri bawah
		 0.7f, -0.7f, 0.6f, 0.2f, 0.0f, 0.8f, 1.0f, // kanan bawah
		 0.7f,  0.7f, 0.6f, 0.2f, 0.0f, 0.8f, 1.0f, // kanan atas
		-0.7f,  0.7f, 0.6f, 0.2f, 0.0f, 0.8f, 1.0f  // kiri atas
	};

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

	m_VertexArraySquare = Karem::VertexArray::CreateVertexArray();
	std::shared_ptr<Karem::VertexBuffer> squareVertexBuffer = Karem::VertexBuffer::CreateVertexBuffer((void*)squareVertices, sizeof(squareVertices));
	std::shared_ptr<Karem::IndexBuffer> squareIndexBuffer = Karem::IndexBuffer::CreateIndexBuffer((void*)squareIndices, 6);
	m_VertexArraySquare->AddVertexBuffer(squareVertexBuffer);
	m_VertexArraySquare->SetIndexBuffer(squareIndexBuffer);
	
	{
		Karem::BufferLayout layout = {
			{ Karem::ShaderDataType::Vec3, "aPos" },
			{ Karem::ShaderDataType::Vec4, "aColor" }
		};
		squareVertexBuffer->SetLayout(layout);
	}

	squareVertexBuffer->ApplyLayout();

	m_VertexArraySquare->UnBind();
}

void AppLayer::OnUpdate()
{
	std::dynamic_pointer_cast<Karem::OpenGLShader>(m_ShaderTriangle)->BindAndUploadUniform();
	Karem::Renderer::Draw(m_VertexArrayTriangle, m_ShaderTriangle);
	Karem::Renderer::Draw(m_VertexArraySquare, m_ShaderSquare);
}

Sandbox::Sandbox()
{
	Init();

	PushLayer(std::make_shared<AppLayer>());
}

Sandbox::~Sandbox()
{
	Shutdown();
}

void Sandbox::Run()
{
	while (m_Running)
	{
		Karem::RendererCommand::Clear();
		Karem::RendererCommand::ClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });

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

			if (i <  m_Layers.GetSize()-1)
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

		for (std::shared_ptr<Karem::Layer>& layer : m_Layers)
		{
			if(layer->GetStatus())
				layer->OnUpdate(); // Memanggil fungsi yang diinginkan dari shared_ptr
		}


		m_Window.OnUpdate();
	}
}

void Sandbox::Init()
{
	m_Window.SetEventCallbacks(std::bind(&Application::EventHandler, this, std::placeholders::_1));

	// ImGUI init
	GLFWwindow* window = glfwGetCurrentContext();

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
