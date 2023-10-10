#pragma once

#include "Core/TimeStep.h"
#include "Event/Event.h"

#include <entt/entt.hpp>

namespace Karem {

	class Scene {
	public:
		Scene();

		entt::entity CreateEntity(const std::string& entityName = "Undefined");

		// this is temporary
		entt::registry& GetRegistry() { return m_Registry; }

		void Update(TimeStep ts);
		void EventHandler(Event& e);
	private:
		entt::registry m_Registry;
		entt::entity m_SquareEntity;
	};
}