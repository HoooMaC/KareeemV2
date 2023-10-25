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

	void Scene::Update(TimeStep ts)
	{
		glm::mat4 cameraTransform(1.0f);
		auto view = m_Registry.view<TransformComponent, CameraComponent>();
		for (const auto entity : view)
		{
			auto [tc, cc] = view.get(entity);
			m_MainCamera = &cc.Camera;
			cameraTransform = tc.GetTransformMatrix();
			break;
		}
		// need to fix
		// consider with the main camera, if there is no camera
		if (m_MainCamera)
		{
			glm::mat4 proj = m_MainCamera->GetCameraProjectionMatrix();
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

	CameraHandler* Scene::GetMainCamera() const
	{
		return m_MainCamera ? m_MainCamera : nullptr;
	}

}