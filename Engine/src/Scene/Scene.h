#pragma once

#include "Core/TimeStep.h"
#include "Event/Event.h"

#include "Renderer/FrameBuffer.h"
#include "Renderer/CameraHandler.h"

#include <entt/entt.hpp>

namespace Karem {

	class Entity;
	class SceneHierarcyPanel;

	class Scene {
		friend class Entity;
		friend class SceneHierarcyPanel;
	public:
		Scene();
		~Scene() = default;

		Entity CreateEntity(const std::string& entityName = "Undefined");

		void Update(TimeStep ts);
		void EventHandler(Event& e);

		CameraHandler* GetMainCamera();
	private:
		entt::registry m_Registry;
	};

}