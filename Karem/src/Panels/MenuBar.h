#pragma once

#include "Event/Event.h"
#include "Event/KeyEvent.h"

namespace Karem {

	class Panels;

	class MenubarPanel
	{
		friend class Panels;
	public:
		void Render(bool* statusArray);
		void EventHandler(Event& event);

	private:
		bool KeyPressedAction(KeyPressedEvent& event);

	private:
		std::function<void()> newSceneFunction;
		std::function<void(std::string_view)> saveSceneAsFunction;
		std::function<void(std::string_view)> openSceneFunction;

	};

}