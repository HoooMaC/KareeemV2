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
		newEntity.AddComponent<TransformComponent>();
		return newEntity;
	}

	void Scene::DestroyEntity(entt::entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::Update(TimeStep ts)
	{
		CameraHandler* mainCamera = nullptr;
		glm::mat4 cameraTransform(1.0f);
		auto view = m_Registry.view<TransformComponent, CameraComponent>();
		for (const auto entity : view)
		{
			auto [tc, cc] = view.get(entity);
			if(cc.MainCamera)
			{
				mainCamera = &cc.Camera;
				cameraTransform = tc.GetTransformMatrix();
				break;
			}
		}

		if (mainCamera)
		{
			glm::mat4 proj = mainCamera->GetCameraProjectionMatrix();
			Renderer::BeginScene(proj, cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<ColorComponent>);
			for (const auto entity : group)
			{
				auto [transform, color] = group.get(entity);
				Renderer::SubmitQuad(transform.GetTransformMatrix(), color.Color);
			}
			Renderer::EndScene();
		}
	}

	void Scene::EventHandler(Event& e)
	{
	}

	void Scene::OnViewportResize(float width, float height)
	{
		const auto& view = m_Registry.view<CameraComponent>();

		for (const auto entity : view)
		{
			auto [cc] = view.get(entity);
			cc.Camera.SetAspectRatio(width / height);
		}

	}


}