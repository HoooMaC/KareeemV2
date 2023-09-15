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
			uint32_t height = 720
		)
			: Title(title), Width(width), Height(height) {}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window(const WindowProperty& property = WindowProperty());
		~Window();

		virtual void Init(const WindowProperty& property);
		virtual void Shutdown();

		virtual void OnUpdate();
		virtual void SetVSync(bool enable);
		virtual void SetEventCallbacks(EventCallbackFn callback);
		virtual void Resize(int width, int height);

		virtual bool IsVSync();
		virtual uint32_t GetWidth() const;
		virtual uint32_t GetHeight() const;
		virtual void* GetNativeWindow() const;
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