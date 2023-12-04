#pragma once

#include "Core/TimeStep.h"
#include "Event/Event.h"

#include "Renderer/FrameBuffer.h"
#include "Renderer/CameraHandler.h"

#include <entt/entt.hpp>

namespace Karem {

	class Entity;
	class SceneHierarcyPanel;
	class SceneSerializer;

	class Scene {
		friend class Entity;
		friend class SceneHierarcyPanel;
		friend class SceneSerializer;
	public:
		Scene();
		~Scene() = default;

		Entity CreateEntity(const std::string& entityName = "Undefined");
		void DestroyEntity(entt::entity entity);

		void Update(TimeStep ts);
		void EventHandler(Event& e);
		
		void OnViewportResize(float width, float height);
		void OnViewportResize(float aspectRatio);
	private:
		entt::registry m_Registry;
	};

}