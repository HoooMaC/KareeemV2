#include "Sandbox.h"

#include <glad/glad.h>

// TEMP : THIS SHOULD NOT BE HERE (MAYBE)
#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

const char* vertexShaderSource = R"(
	#version 330 core
	layout (location = 0) in vec2 aPos;
	void main()
	{
		gl_Position = vec4(aPos, 0.0, 1.0);
	}
)";

const char* fragmentShaderSource = R"(
	#version 330 core
	out vec4 FragColor;
	void main()
	{
		FragColor = vec4(1.0, 0.5, 0.2, 1.0);
	}
)";

Karem::Application* Karem::CreateApplication()
{
	return new Sandbox();
}

GLenum OpenGLToShaderDataType(Karem::ShaderDataType type)
{
	switch (type)

	{
		case Karem::ShaderDataType::None:
			return 0;
		case Karem::ShaderDataType::Float:
			return GL_FLOAT;
		case Karem::ShaderDataType::Vec2:
			return GL_FLOAT;
		case Karem::ShaderDataType::Vec3:
			return GL_FLOAT;
		case Karem::ShaderDataType::Vec4:
			return GL_FLOAT;
		case Karem::ShaderDataType::Mat2:
			return GL_FLOAT;
		case Karem::ShaderDataType::Mat3:
			return GL_FLOAT;
		case Karem::ShaderDataType::Mat4:
			return GL_FLOAT;
		case Karem::ShaderDataType::Int:
			return GL_INT;
		case Karem::ShaderDataType::Int2:
			return GL_INT;
		case Karem::ShaderDataType::Int3:
			return GL_INT;
		case Karem::ShaderDataType::Int4:
			return GL_INT;
		case Karem::ShaderDataType::Bool:
			return GL_BOOL;
	}
	// TO DO : NEED SOME ASSERTION HERE
}

void AppLayer::OnAttach()
{
	// Compile dan link shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	// TO DO : need to check the compile status

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	// TO DO : need to check the compile status

	m_ShaderProgram = glCreateProgram();
	glAttachShader(m_ShaderProgram, vertexShader);
	glAttachShader(m_ShaderProgram, fragmentShader);
	glLinkProgram(m_ShaderProgram);
	// TO DO : need to check the link status

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float vertices[2 * 3] =
	{
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.0f, 0.5f
	};

	unsigned int indices[3] = { 0, 1, 2 };


	m_VArray = Karem::CreateVertexArray();
	m_VBuffer = Karem::CreateVertexBuffer((void*)vertices, sizeof(vertices));
	m_IBuffer = Karem::CreateIndexBuffer((void*)indices, 3);


	{
		Karem::BufferLayout layout = {
			{ Karem::ShaderDataType::Vec2, "position" }
		};

		m_VBuffer->SetLayout(layout);
	}
		
	int32_t index = 0;
	for (auto& element : m_VBuffer->GetLayout())
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(
			index,
			element.GetCount(),
			OpenGLToShaderDataType(element.Type),
			element.Normalized ? GL_TRUE : GL_FALSE,
			element.Size,
			(const void*)element.Offset
		);
		index++;
	}


	m_IBuffer->UnBind();
	m_VBuffer->UnBind();
	m_VArray->UnBind();
}

void AppLayer::OnUpdate()
{
	glUseProgram(m_ShaderProgram);

	m_IBuffer->Bind();
	m_VArray->Bind();

	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
}

Sandbox::Sandbox()
{
	Init();

	PushLayer(std::make_shared<AppLayer>());
	PushLayer(std::make_shared<Test1Layer>("Test1"));
	PushLayer(std::make_shared<Test1Layer>("Test2"));
	PushLayer(std::make_shared<Test1Layer>("Test3"));
	PushLayer(std::make_shared<Test1Layer>("Test4"));
	PushLayer(std::make_shared<Test1Layer>("Test5"));
	PushLayer(std::make_shared<Test1Layer>("Test6"));
	PushLayer(std::make_shared<Test1Layer>("Test7"));
}

Sandbox::~Sandbox()
{
	Shutdown();
}

void Sandbox::Run()
{
	while (m_Running)
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

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
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsLight();

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
