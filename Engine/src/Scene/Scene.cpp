#include "Core/Kareeem.h"
#include "Scene.h"

#include "Core/CoreFunction.h"

#include "Scene/Components.h"
#include "Scene/Entity.h"

#include "Renderer/Renderer.h"

#include <glm/glm.hpp>

#include <imgui.h>

namespace Karem{

	Scene::Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& entityName)
	{
		Entity newEntity{ m_Registry.create(), this };
		newEntity.AddComponent<TagComponent>(entityName.c_str());
		newEntity.AddComponent<TransformComponent>(glm::mat4(1.0f));
		return newEntity;
	}

	void Scene::Update(TimeStep ts)
	{
		CameraHandler* activeCamera = nullptr;
		glm::mat4 cameraTransform(1.0f);
		auto view = m_Registry.view<TransformComponent, CameraComponent>();
		for (const auto entity : view)
		{
			auto [tc, cc] = view.get(entity);
			activeCamera = &cc.Camera;
			cameraTransform = tc.Transform;
			break;
		}

		if (activeCamera)
		{
			glm::mat4 proj = activeCamera->GetCameraProjectionMatrix();
			Renderer::BeginScene(proj, cameraTransform);

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
	}
}