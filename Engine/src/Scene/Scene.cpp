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
		m_FrameBuffer = CreateFrameBuffer(1280, 720);
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

		auto view = m_Registry.view<CameraComponent>();
		for (const auto entity : view)
		{
			auto [component] = view.get(entity);
			activeCamera = &component.Camera;
			break;
		}

		if (activeCamera)
		{
			void* camera = activeCamera->GetCamera(CameraType::Orthographic);
			auto type = activeCamera->GetCurrentCameraType();
			if(type == CameraType::Orthographic)
			{
				auto& data = *(OrthographicCamera*)(camera);
				Renderer::BeginScene(data.GetViewProjectionMatrix());
			}
			else
			{
				auto& data = *(PerspectiveCamera*)(camera);
				Renderer::BeginScene(data.GetViewProjectionMatrix());
			}


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