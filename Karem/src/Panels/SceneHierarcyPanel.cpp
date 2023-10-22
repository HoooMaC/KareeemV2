#include "SceneHierarcyPanel.h"

#include "Scene/Components.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_setup.h>

namespace Karem {

	void SceneHierarcyPanel::RenderImGUI()
	{
		ViewportPanel();
		EntityListPanel();
		EntityPropertiesPanel();
	}

	void SceneHierarcyPanel::ViewportPanel()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("Viewport");
		CameraHandler* activeCamera = nullptr;

		auto view = m_ContextScene->m_Registry.view<CameraComponent>();
		for (const auto entity : view)
		{
			auto [component] = view.get(entity);
			activeCamera = &component.Camera;
		}

		if (activeCamera)
		{
			ImVec2 currentPannelSize = ImGui::GetContentRegionAvail();
			void* camera = activeCamera->GetCamera();
			if (activeCamera->IsOrthographic())
			{
				auto& orthographicCamera = *(OrthographicCamera*)(camera);
				const auto& [fbWidth, fbHeight] = m_ContextScene->m_FrameBuffer->GetFrameBufferSize();

				// TO DO : CHANGE THIS ACCORDING TO THE CAMERA
				if (currentPannelSize.x != fbWidth or currentPannelSize.y != fbHeight)
				{
					ENGINE_DEBUG("Changing the aspect ratio of the camera {} | {}", currentPannelSize.x, currentPannelSize.y);
					orthographicCamera.MaintainAspectRatio(currentPannelSize.x / currentPannelSize.y);
					orthographicCamera.RecalculateNewZoom({ currentPannelSize.x,currentPannelSize.y });
					//orthographicCamera.SetAspectRatio(*(glm::vec2*)&currentPannelSize);

					m_ContextScene->m_FrameBuffer->Resize((int32_t)currentPannelSize.x, (int32_t)currentPannelSize.y);
				}

			}
			else
			{
				auto& perspectiveCamera = *(PerspectiveCamera*)(camera);
				const auto& [fbWidth, fbHeight] = m_ContextScene->m_FrameBuffer->GetFrameBufferSize();

				// TO DO : CHANGE THIS ACCORDING TO THE CAMERA
				if (currentPannelSize.x != fbWidth or currentPannelSize.y != fbHeight)
				{
					m_ContextScene->m_FrameBuffer->Resize((int32_t)currentPannelSize.x, (int32_t)currentPannelSize.y);
					perspectiveCamera.SetAspectRatio(currentPannelSize.x / currentPannelSize.y);
				}
			}
			ImGui::Image(
				(void*)m_ContextScene->m_FrameBuffer->GetTextureColorAttachmentID(),
				{ (float)currentPannelSize.x, (float)currentPannelSize.y },
				ImVec2(0, 1),
				ImVec2(1, 0)
			);
		}

		ImGui::End();

		ImGui::PopStyleVar(3);
	}

	void SceneHierarcyPanel::EntityPropertiesPanel()
	{
		ImGui::Begin("Selected Entity Properties");
		// the problem is here
		if (m_SelectedEntity)
			DrawEntityTreeNode(m_SelectedEntity);

		ImGui::End();
	}

	void SceneHierarcyPanel::EntityListPanel()
	{
		ImGui::Begin("Entity List");
		m_ContextScene->m_Registry.view<TagComponent>().each([&](entt::entity entity, TagComponent& tag) {
			const std::string& tagName = tag.Tag;
			if (ImGui::Selectable(("Entity : " + tagName).c_str()))
				m_SelectedEntity = { entity, m_ContextScene.get()};
		});

		if (ImGui::IsMouseDown(0) and ImGui::IsWindowHovered())
			m_SelectedEntity = {};

		ImGui::Separator();

		if(m_SelectedEntity)
		{
			const char* label = m_SelectedEntity.GetComponent<TagComponent>().Tag.c_str();
			ImGui::Text("Selected : %s", label);
		}

		ImGui::End();
	}

	void SceneHierarcyPanel::DrawEntityTreeNode(Entity entity)
	{

		const char* tag = entity.GetComponent<TagComponent>().Tag.c_str();
		char tagBuffer[256];
		strcpy_s(tagBuffer, sizeof(tagBuffer), tag);
		if (ImGui::InputText("##InputText", tagBuffer, IM_ARRAYSIZE(tagBuffer)))
			tag = tagBuffer;
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag);
		if (opened)
		{
			if (m_SelectedEntity.IsHasComponent<TransformComponent>())
			{
				bool componentOpened = ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), flags, "Transform");
				if (componentOpened)
				{
					TransformComponent& transform = m_SelectedEntity.GetComponent<TransformComponent>();
					ImGui::DragFloat3("Transform", glm::value_ptr(transform.Transform[3]), 0.1f, -10.0f, 20.0f, "%.2f");
					ImGui::TreePop();
				}

			}

			if (m_SelectedEntity.IsHasComponent<ColorComponent>())
			{
				bool componentOpened = ImGui::TreeNodeEx((void*)typeid(ColorComponent).hash_code(), flags, "Color");
				if (componentOpened)
				{
					ColorComponent& color = m_SelectedEntity.GetComponent<ColorComponent>();
					ImGui::ColorEdit4("Color", glm::value_ptr(color.Color));
					ImGui::TreePop();
				}

			}

			if (m_SelectedEntity.IsHasComponent<CameraComponent>())
			{
				bool componentOpened = ImGui::TreeNodeEx((void*)typeid(ColorComponent).hash_code(), flags, "Camera");
				if (componentOpened)
				{
					CameraHandler& camera = m_SelectedEntity.GetComponent<CameraComponent>().Camera;
					if(ImGui::Button("Change Camera Projection"))
						camera.ChangeCameraType();

					if (ImGui::BeginCombo("Projection", camera.GetCameraStringType()))
					{
						bool isOrthographicSelected = "Orthographic" == camera.GetCameraStringType() ? true : false;
						bool isPerspectiveSelected = "Perspective" == camera.GetCameraStringType() ? true : false;
						if (ImGui::Selectable("Orthographic", isOrthographicSelected))
						{
							camera.SetToOrthographic();
						}

						if (ImGui::Selectable("Perspective", isPerspectiveSelected))
						{
							camera.SetToPerspective();
						}

						ImGui::EndCombo();
					}

					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}



	}

}