#pragma once

#include "Core/Kareeem.h"

#include "Event/Event.h"

namespace Karem {

	class Layer
	{
	public:
		Layer(const std::string& name);
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnImGUIRender() {}

		virtual void EventHandler(Event& event) {}
	protected:
		std::string m_DebugName;
	};

}