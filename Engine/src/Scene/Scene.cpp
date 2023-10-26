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
		CameraHandler* mainCamera = nullptr;
		glm::mat4 cameraTransform(1.0f);
		auto view = m_Registry.view<TransformComponent, CameraComponent>();
		for (const auto entity : view)
		{
			auto [tc, cc] = view.get(entity);
			if(cc.mainCamera)
			{
				mainCamera = &cc.Camera;
				cameraTransform = tc.GetTransformMatrix();
				break;
			}
		}
		// need to fix
		// consider with the main camera, if there is no camera
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

	CameraHandler* Scene::GetMainCamera()
	{
		CameraHandler* mainCamera = nullptr;
		const auto& view = m_Registry.view<TransformComponent, CameraComponent>();

		for (const auto entity : view)
		{
			auto [tc, cc] = view.get(entity);
			auto& camera = cc.Camera;

			if (cc.mainCamera)  // Anda mungkin perlu memiliki metode untuk memeriksa apakah ini adalah kamera utama
			{
				mainCamera = &camera;
				break;  // Hentikan loop saat menemukan kamera utama
			}
		}

		return mainCamera;
	}

}