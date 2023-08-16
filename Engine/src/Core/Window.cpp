#include "Core/Kareeem.h"
#include "Window.h"

namespace Karem {

	static bool s_Initialized = false;

	Window::Window(const WindowProperty& property)
	{
		Init(property);
	}

	Window::~Window()
	{
		Shutdown();
	}

	void Window::Init(const WindowProperty& property)
	{
		m_Data.Title = property.Title;
		m_Data.Width = property.Width;
		m_Data.Height = property.Height;

		if (!s_Initialized)
		{
			int succes = glfwInit();

			if (!succes)
				std::cout << "Failed to Initialized GLFW\n";

			s_Initialized = true;
		}

		m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
	}

	void Window::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void Window::OnUpdate()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}

	void Window::SetVSync(bool enable)
	{
		if (enable)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enable;
	}

	void Window::SetEventCallbacks(EventCallbackFn callback)
	{
		m_Data.EventCallback = callback;
	}

	bool Karem::Window::IsVSync()
	{
		return m_Data.VSync;
	}

	uint32_t Window::GetWidth() const
	{
		return m_Data.Width;
	}

	uint32_t Window::GetHeight() const
	{
		return m_Data.Height;
	}

	void* Window::GetNativeWindow() const
	{
		return m_Window;
	}

}