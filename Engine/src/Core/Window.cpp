#include "Core/Kareeem.h"

#include "Window.h"

#include "API/OpenGL/OpenGLContext.h"

#include "Event/Event.h"
#include "Event/AppEvent.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"

#include "Platform/Windows/Platform.h"

#include "imgui_setup.h"

#include <GLFW/glfw3.h>

// this is temporary
#include <glad/glad.h>

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

			ENGINE_ASSERT(succes, "Failed to Initialized GLFW");

			//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		}

		m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		m_Context = std::make_unique<OpenGLContext>();

		if (!s_Initialized)
		{
			m_Context->Init();

			s_Initialized = true;
		}

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(data.Width, data.Height);
				data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				WindowCloseEvent event;
				data.EventCallback(event);
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			
			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
			//ENGINE_TRACE(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});

		// belum ada key type event
		//glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int codepoint)
		//{
		//	WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		//	KeyTypedEvent event(codepoint);
		//	data.EventCallback(event);
		//});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancodes, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
		});

	}

	void Window::Shutdown()
	{
		if(s_Initialized)
		{

			glfwDestroyWindow(m_Window);
			glfwTerminate();

			s_Initialized = false;
		}
	}

	void Window::Update()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
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

	void Window::Resize(int width, int height)
	{
		m_Data.Width = width;
		m_Data.Height = height;
		Platform::ResizeWindow(m_Window, width, height);
	}

	void Window::SetFullScreen()
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor(); // Menggunakan monitor utama
		const GLFWvidmode* mode = glfwGetVideoMode(monitor); // Mendapatkan mode video monitor

		glfwSetWindowMonitor(m_Window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		glViewport(0, 0, mode->width, mode->height);
	}

	bool Window::IsFullScreen()
	{
		int currentWidth, currentHeight;
		glfwGetWindowSize(m_Window, &currentWidth, &currentHeight);

		GLFWmonitor* monitor = glfwGetPrimaryMonitor(); // Menggunakan monitor utama
		const GLFWvidmode* mode = glfwGetVideoMode(monitor); // Mendapatkan mode video monitor

		ENGINE_DEBUG("current size : {}\t{}", currentWidth, currentHeight);
		ENGINE_DEBUG("monitor size : {}\t{}", mode->width, mode->height);

		if (mode->width == currentWidth and mode->height == currentHeight)
			return true;

		return false;
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