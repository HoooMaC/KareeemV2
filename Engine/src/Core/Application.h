#pragma once

namespace Karem {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void Run();

	private:
		void Init();
		void Shutdown();

		//void EventHandler();

	private:
		bool m_Running = true;
	};

	Application* CreateApplication();

}
