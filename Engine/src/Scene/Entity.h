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

		template<typename T, typename ...Args>
		inline T& AddComponent(Args&& ...args)
		{
			bool isHas = IsHasComponent<T>();
			if (isHas)
			{
				ENGINE_WARN("This entity already have the component");	
				return m_SceneReference->m_Registry.get<T>(m_EntityId);
			}
			return m_SceneReference->m_Registry.emplace<T>(m_EntityId, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			ENGINE_ASSERT(IsHasComponent<T>(), "The entity doesn't have the component");
			return m_SceneReference->m_Registry.get<T>(m_EntityId);
		}

		template<typename T>
		void RemoveComponent()
		{
			if(IsHasComponent<T>())
				m_SceneReference->m_Registry.remove<T>(m_EntityId);
		}

		template<typename ...Args>
		bool IsHasComponent()
		{
			return m_SceneReference->m_Registry.all_of<Args...>(m_EntityId);
		}
	
		operator uint32_t() { return (uint32_t)m_EntityId; }
		operator bool() { return m_EntityId != entt::null; }
		operator bool() const { return m_EntityId != entt::null; }


		bool operator==(const Entity& other) const { return m_EntityId == other.m_EntityId and m_SceneReference == other.m_SceneReference; }
		bool operator!= (const Entity& other) const { return !operator==(other); }

	private:
		entt::entity m_EntityId{ entt::null };
		Scene* m_SceneReference = nullptr;
	};

}
 