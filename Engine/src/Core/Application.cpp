#include "Core/Kareeem.h"

#include "Application.h"

#include "Renderer/Renderer2D.h"

namespace Karem {

	Application::Application(const WindowProperty& props) : m_Window(props)
	{
		m_Window.SetEventCallbacks(std::bind(&Application::EventHandler, this, std::placeholders::_1));
		GLFWwindow* window = Karem::GraphicsContext::GetContextCurrent();
		imgui::InitializeImGUI(window);

		Renderer2D::Initialize();
	}

	Application::~Application()
	{
		Renderer2D::Shutdown();
	}

}