#pragma once
#include "Core/Layer/Layer.h"

namespace Karem {

	class ImGUILayer : public Layer
	{
		ImGUILayer();
		~ImGUILayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnImGUIRender();

		void EventHandler(Event& event);
	};

}

