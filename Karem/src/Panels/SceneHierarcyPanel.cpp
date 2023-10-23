#include "SceneHierarcyPanel.h"

#include "Scene/Components.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_setup.h>

namespace Karem {

	void SceneHierarcyPanel::RenderImGUI()
	{
		EntityListPanel();
		EntityPropertiesPanel();
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
				m_SelectedEntity = { entity, m_ContextScene.get() };
			});

		if (ImGui::IsMouseDown(0) and ImGui::IsWindowHovered())
			m_SelectedEntity = {};

		ImGui::Separator();

		if (m_SelectedEntity)
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
					if (ImGui::Button("Change Camera Projection"))
						camera.ChangeCameraType();

					if (ImGui::BeginCombo("Projection", camera.GetCameraStringType()))
					{
						bool isOrthographicSelected = "Orthographic" == camera.GetCameraStringType() ? true : false;
						bool isPerspectiveSelected = "Perspective" == camera.GetCameraStringType() ? true : false;
						if (ImGui::Selectable("Orthographic", isOrthographicSelected))
						{
							camera.SetTypeToOrthographic();
						}

						if (ImGui::Selectable("Perspective", isPerspectiveSelected))
						{
							camera.SetTypeToPerspective();
						}
						ImGui::EndCombo();
					}

					const char* sizeLabel = "size";
					const char* nearLabel = "Near";
					const char* farLabel = "far";

					if (camera.IsOrthographic())
					{
						float orthoSize = camera.GetOrthographicSize();
						if (ImGui::DragFloat("Ortho size", &orthoSize))
							camera.SetOrthographicSize(orthoSize);
					}
					//ImGui::DragFloat(sizeLabel,);
					float nearClip = camera.GetCameraNear();
					float farClip = camera.GetCameraFar();

					if (ImGui::DragFloat("Near Clip", &nearClip))
						camera.SetCameraNear(nearClip);

					if (ImGui::DragFloat("Far Clip", &farClip))
						camera.SetCameraFar(farClip);

					if (camera.IsOrthographic())
					{
						const auto& [left, right, bottom, top] = camera.GetCameraBounds();
						float orthoSize = camera.GetOrthographicSize();

						ImGui::Separator();
						ImGui::Text("Left : %f", left);
						ImGui::Text("Right : %f", right);
						ImGui::Text("Bottom : %f", bottom);
						ImGui::Text("Top : %f", top);
						ImGui::Text("Near : %f", nearClip);
						ImGui::Text("Far : %f", farClip);
						ImGui::Text("Size : %f", orthoSize);
					}

					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}



	}

}