#pragma once

#include <Scene/Components.h>
#include "Scene/Scene.h"

#include <entt/entt.hpp>

namespace Karem {
	

	class Entity
	{
	public:
		Entity() = default;
		Entity(const Entity&) = default;
		Entity(entt::entity ent, Scene* ref)
			: m_EntityId(ent), m_SceneReference(ref) {}

		TransformComponent& GetTransformComponent()
		{
			return m_SceneReference->m_Registry.get<TransformComponent>(m_EntityId);
		}

		template<typename T, typename ...Args>
		inline T& AddComponent(Args&& ...args)
		{
			return m_SceneReference->m_Registry.emplace<T>(m_EntityId, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			return m_SceneReference->m_Registry.get<T>(m_EntityId);
		}

		template<typename T>
		void RemoveComponent()
		{
			m_SceneReference->m_Registry.remove<T>(m_EntityId);
		}

		template<typename ...Args>
		bool IsHasComponent()
		{
			return m_SceneReference->m_Registry.all_of<Args...>(m_EntityId);
		}

	private:
		entt::entity m_EntityId{ entt::null };
		Scene* m_SceneReference = nullptr;
	};

}
