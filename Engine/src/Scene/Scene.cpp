#include "Core/Kareeem.h"
#include "Scene.h"

#include "Core/CoreFunction.h"

#include "Scene/Components.h"
#include "Scene/Entity.h"

#include "Renderer/Renderer.h"

#include <glm/glm.hpp>

namespace Karem{

	Scene::Scene()
	{
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
		CameraHandler* mainCamera = nullptr;

		auto view = m_Registry.view<CameraComponent>();
		for (const auto entity : view)
		{
			auto [component] = view.get(entity);
			mainCamera = &component.Camera;
			break;
		}

		if (mainCamera)
		{
			Renderer::BeginScene(mainCamera->GetCamera(CameraHandler::CameraType::Orthographic));
			auto group = m_Registry.group<TransformComponent>(entt::get<ColorComponent>);
			for (const auto entity : group)
			{
				auto [transform, color] = group.get(entity);
				Renderer::SubmitQuad(transform.Transform, color.Color);
			}
			Renderer::EndScene();
		}
	}

	void Scene::EventHandler(Event& e)
	{
		auto view = m_Registry.view<CameraComponent>();
		for (const auto entity : view)
		{
			auto [component] = view.get(entity);
			component.Camera.EventHandler(e);
			break;
		}
	}


}