#pragma once

#include "Event/Event.h"

#include "Renderer/GraphicsContext.h"

#include <functional>
#include <string>

struct GLFWwindow;

namespace Karem {

	struct WindowProperty
	{
		std::string Title;
		uint32_t Width, Height;

		WindowProperty(
			const std::string& title = "Default Window",
			uint32_t width = 1280,
			uint32_t height = 720,
			bool fullScreen = false
		)
			: Title(title), Width(width), Height(height) {}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window(const WindowProperty& property = WindowProperty());
		~Window();

		void Init(const WindowProperty& property);
		void Shutdown();

		void OnUpdate();
		void SetVSync(bool enable);
		void SetEventCallbacks(EventCallbackFn callback);

		void Resize(int width, int height);
		void SetFullScreen();
		bool IsFullScreen();

		bool IsVSync();
		uint32_t GetWidth() const;
		uint32_t GetHeight() const;
		void* GetNativeWindow() const;
	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			uint32_t Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;

		std::unique_ptr<GraphicsContext> m_Context;
	};

}