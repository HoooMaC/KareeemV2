#pragma once

#include "Core/Window.h"
#include "Core/Layer/Layers.h"

#include "Event/Event.h"
#include "Event/AppEvent.h"

namespace Karem {

	class Application
	{
	public:
		virtual ~Application() {}

		virtual void Run() = 0;

		virtual void EventHandler(Event& event) = 0;

	protected:
		virtual void Init() = 0;
		virtual void Shutdown() = 0;

	};

	Application* CreateApplication();

}
