#pragma once

#include "Core/TimeStep.h"
#include "Event/Event.h"

#include <entt/entt.hpp>

namespace Karem {

	class Entity;

	class Scene {
		friend class Entity;
	public:
		Scene();

		Entity CreateEntity(const std::string& entityName = "Undefined");

		void Update(TimeStep ts);
		void EventHandler(Event& e);
		void RenderImGUI();
	private:
		entt::registry m_Registry;
	};
	
}