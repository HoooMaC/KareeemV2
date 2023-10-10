#pragma once
#include "Layer/Layer.h"

namespace Karem {

	class ImGUILayer : public Layer
	{
	public:
		ImGUILayer();
		~ImGUILayer();

		void OnAttach();
		void OnDetach();
		void Update();
		void RenderImGUI();

		void EventHandler(Event& event);
	};

}