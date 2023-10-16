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
			void* camera = mainCamera->GetCamera(CameraType::Orthographic);
			auto type = mainCamera->GetCurrentCameraType();
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

	void Scene::RenderImGUI()
	{
		ImGui::Begin("Current Properties");
		{
			auto view = m_Registry.view<CameraComponent>();
			for (const auto entity : view)
			{
				auto [component] = view.get(entity);

				if (ImGui::Button("ChangeCamera"))
				{
					auto& Camera = component.Camera;
					switch (Camera.GetCurrentCameraType())
					{
					case CameraType::Orthographic:
						Camera.SetCurrrentCamera(CameraType::Perspective);
						break;
					case CameraType::Perspective:
						Camera.SetCurrrentCamera(CameraType::Orthographic);
						break;
					}
				}
			}
		}

		ImGui::Text("Entities");
		ImGui::Separator();
		{
			auto view = m_Registry.view<TagComponent, TransformComponent, ColorComponent>();
			for (auto entity : view)
			{
				auto [tag, transform, color] = view.get(entity);

				char tagBuffer[256];
				strcpy_s(tagBuffer, tag.Tag.c_str());
				if (ImGui::InputText("##InputText", tagBuffer, IM_ARRAYSIZE(tagBuffer))) {
					tag.Tag = tagBuffer;
				}

				ImGui::DragFloat3("Transform", glm::value_ptr(transform.Transform[3]), 0.1f, -10.0f, 20.0f, "%.2f");

				ImGui::ColorEdit4("Color", glm::value_ptr(color.Color));
			}
		}
		ImGui::End();
		static bool show = true;
		ImGui::ShowDemoWindow(&show);
	}


}