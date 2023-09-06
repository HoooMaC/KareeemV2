#include "Core/Kareeem.h"

#include "Application.h"

namespace Karem {

	Application::Application(const WindowProperty& props) : m_Window(props)
	{
		m_Window.SetEventCallbacks(std::bind(&Application::EventHandler, this, std::placeholders::_1));
		GLFWwindow* window = Karem::GraphicsContext::GetContextCurrent();
		imgui::InitializeImGUI(window);
	}

}