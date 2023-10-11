#include "Core/Kareeem.h"
#include "Scene.h"

#include "Core/CoreFunction.h"

#include "Scene/Components.h"
#include "Scene/Entity.h"

#include "Renderer/Renderer2D.h"

#include <glm/glm.hpp>

namespace Karem{

	Scene::Scene()
	{
		for (int i = 0; i < 10; i++)
		{
			Entity SquareEntity = CreateEntity("Square Entity");
			glm::vec4 color = { 0.4f, 0.0f, (float)i/10, 1.0f};
			SquareEntity.AddComponent<ColorComponent>(color);
			auto& SquareTransform = SquareEntity.GetComponent<TransformComponent>().Transform;
			SquareTransform = glm::translate(glm::mat4(1.0f), { (float)i - 5, 0.0f, 0.0f }) * glm::scale(glm::mat4(1.0f), { 10.0, 10.0f, 0.0f });
		}

	}

	Entity Scene::CreateEntity(const std::string& entityName)
	{
		Entity newEntity{ m_Registry.create(), this };
		newEntity.AddComponent<TransformComponent>(glm::mat4(1.0f));
		newEntity.AddComponent<TagComponent>(entityName.c_str());
		return newEntity;
	}

	void Scene::Update(TimeStep ts)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<ColorComponent>);
		for (const auto entity : group)
		{
			auto [transform, color] = group.get(entity);
			Renderer2D::SubmitQuad(transform.Transform, color.Color);
		}
	}

	void Scene::EventHandler(Event& e)
	{
	}

}