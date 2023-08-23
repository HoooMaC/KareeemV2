#include "Core/Kareeem.h"

#include "Application.h"

#include "Core/Layer/ImGUILayer.h"

#include <Event/Event.h>
#include <Event/AppEvent.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

namespace Karem {

	Application::Application()
	{
		Init();
	}

	void Application::Init()
	{
		m_Window.SetEventCallbacks(std::bind(&Application::EventHandler, this, std::placeholders::_1));

		// Compile dan link shader
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
		glCompileShader(vertexShader);

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
		glCompileShader(fragmentShader);

		m_ShaderProgram = glCreateProgram();
		glAttachShader(m_ShaderProgram, vertexShader);
		glAttachShader(m_ShaderProgram, fragmentShader);
		glLinkProgram(m_ShaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		float vertices[2 * 3] =
		{
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.0f, 0.5f
		};

		unsigned int indices[3] = { 0, 1, 2 };

		glGenVertexArrays(1, &m_VertexArray);
		glGenBuffers(1, &m_VertexBuffer);
		glGenBuffers(1, &m_IndexBuffer);

		glBindVertexArray(m_VertexArray);

		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}

	Application::~Application()
	{
		Shutdown();
	}

	void Application::Shutdown()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			ImGUILayer::Begin();
			for (std::shared_ptr<Layer>& layer : m_Layers)
			{
				layer->OnUpdate(); // Memanggil fungsi yang diinginkan dari shared_ptr
			}
			ImGUILayer::End();

			glUseProgram(m_ShaderProgram);
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

			m_Window.OnUpdate();
		}
	}

	void Application::EventHandler(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::WindowCloseAction, this, std::placeholders::_1));

		for (auto it = m_Layers.rbegin(); it != m_Layers.rend(); ++it)
		{
			(*it)->EventHandler(event); // Memanggil fungsi yang diinginkan dari shared_ptr
		}

		//ENGINE_TRACE(event);
	}

	bool Application::WindowCloseAction(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}
}