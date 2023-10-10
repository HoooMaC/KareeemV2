#pragma once

#include "Core/Kareeem.h"
#include "Core/TimeStep.h"

#include "Event/Event.h"

namespace Karem {

	class Layer
	{
	public:
		Layer(std::string_view name);
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void Update(TimeStep ts) {}
		virtual void RenderImGUI() {}
		virtual void EventHandler(Event& event) {}

		inline bool GetStatus() const { return m_Status; }
		inline std::pair<std::string&, bool&> GetLayerData() { return { m_DebugName, m_Status }; }

	protected:
		bool m_Status;
		std::string m_DebugName;
	};

}