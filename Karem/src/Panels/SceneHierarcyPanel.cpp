#include "SceneHierarcyPanel.h"

#include "Scene/Components.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_internal.h>
//#include <imgui_setup.h>

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

		if (ImGui::BeginPopupContextWindow("Create new entity", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_ContextScene->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}

		ImGui::Separator();

		if (m_SelectedEntity)
		{
			const char* label = m_SelectedEntity.GetComponent<TagComponent>().Tag.c_str();
			ImGui::Text("Selected : %s", label);
		}

		ImGui::End();
	}

	static void DrawFloat3Component(const char* label, glm::vec3& values, float resetValue = 0.0, float columnWidth = 100.0f)
	{
		auto* g = ImGui::GetCurrentContext();
		ImGuiTable* table = g->CurrentTable;
		if (table)
		{

			ImGui::PushID(label);

			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::Text(label);

			ImGui::TableSetColumnIndex(1);

			ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0,0 });

			ImGui::GetFontSize();
			float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2;
			const ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			if (ImGui::Button("X", buttonSize))
				values.x = resetValue;
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			if (ImGui::Button("Y", buttonSize))
				values.y = resetValue;
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			if (ImGui::Button("Z", buttonSize))
				values.z = resetValue;
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();

			ImGui::PopStyleVar();


			ImGui::PopID();
		}
		else
		{
			ImGui::PushID(label);

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, columnWidth);
			ImGui::Text(label);
			ImGui::NextColumn();

			ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0,0 });

			ImGui::GetFontSize();
			float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2;
			const ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			if (ImGui::Button("X", buttonSize))
				values.x = resetValue;
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			if (ImGui::Button("Y", buttonSize))
				values.y = resetValue;
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			if (ImGui::Button("Z", buttonSize))
				values.z = resetValue;
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();

			ImGui::PopStyleVar();

			ImGui::Columns(1);

			ImGui::PopID();

		}
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(std::string_view label, Entity entity, UIFunction uiFunction)
	{
		static constexpr ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen 
			| ImGuiTreeNodeFlags_Framed 
			| ImGuiTreeNodeFlags_FramePadding 
			| ImGuiTreeNodeFlags_AllowItemOverlap
			| ImGuiTreeNodeFlags_SpanAvailWidth;

		if (entity.IsHasComponent<T>())
		{
			T& component = entity.GetComponent<T>();
			ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 4,4 });
			const auto contentRegionAvail = ImGui::GetContentRegionAvail();
			const auto fontSize = ImGui::GetFontSize();
			const auto& style = ImGui::GetStyle();
			const float lineHeight = fontSize + style.FramePadding.y * 2.0f;
			ImVec2 buttonSize = { lineHeight, lineHeight };


			ImGui::Separator();
			bool componentOpened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, label.data());
			ImGui::PopStyleVar();

			std::string popupLabel = "RemoveComponent";
			popupLabel.insert(6, label.data());

			ImGui::SameLine(contentRegionAvail.x + lineHeight / 2.0f);
			if (ImGui::Button("-", buttonSize ))
				ImGui::OpenPopup(popupLabel.c_str());
			ImGui::Separator();

			bool isComponentDeleted = false;
			if (ImGui::BeginPopup(popupLabel.c_str(), ImGuiWindowFlags_NoMove))
			{
				if (ImGui::MenuItem("Remove"))
				{
					isComponentDeleted = true;
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			if (componentOpened)
			{
				uiFunction(component);
				ImGui::TreePop();
			}
			ImGui::PopStyleVar();

			if (isComponentDeleted)
			{
				entity.RemoveComponent<T>();
			}
		}
	}

	void SceneHierarcyPanel::DrawEntityTreeNode(Entity entity)
	{
		static constexpr ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen
			| ImGuiTreeNodeFlags_Framed
			| ImGuiTreeNodeFlags_FramePadding
			| ImGuiTreeNodeFlags_OpenOnArrow
			| ImGuiTreeNodeFlags_OpenOnDoubleClick
			| ImGuiTreeNodeFlags_AllowItemOverlap
			| ImGuiTreeNodeFlags_SpanAvailWidth;

		const auto contentRegionAvail = ImGui::GetContentRegionAvail();

		const auto fontSize = ImGui::GetFontSize();
		const auto& style = ImGui::GetStyle();
		const float lineHeight = fontSize + style.FramePadding.y * 2.0f;
		const ImVec2 buttonSize = { lineHeight, lineHeight };

		std::string& tag = entity.GetComponent<TagComponent>().Tag;
		char tagBuffer[256];
		strcpy_s(tagBuffer, sizeof(tagBuffer), tag.c_str());

		ImGui::Text("Tag");


		ImGui::SameLine();
		if (ImGui::InputText("##InputText", tagBuffer, IM_ARRAYSIZE(tagBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
			tag = tagBuffer;

		ImGui::SameLine(contentRegionAvail.x - buttonSize.x * 0.5f);
		if (ImGui::Button("+", buttonSize))
			ImGui::OpenPopup("Add Component");

		if (ImGui::BeginPopup("Add Component", ImGuiWindowFlags_NoMove))
		{
			if (!m_SelectedEntity.IsHasComponent<TransformComponent>())
			{
				if (ImGui::MenuItem("Transform"))
				{
					m_SelectedEntity.AddComponent<TransformComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!m_SelectedEntity.IsHasComponent<ColorComponent>())
			{
				if (ImGui::MenuItem("Color"))
				{
					m_SelectedEntity.AddComponent<ColorComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!m_SelectedEntity.IsHasComponent<CameraComponent>())
			{
				if (ImGui::MenuItem("Camera"))
				{
					m_SelectedEntity.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::EndPopup();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 4,4 });


		ImGui::Separator();
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		ImGui::PopStyleVar();



		if (opened)
		{
			DrawComponent<TransformComponent>("Transform", m_SelectedEntity, [](TransformComponent& component) 
				{
					static constexpr ImGuiTableFlags transfromTableFlags = ImGuiTableFlags_BordersInnerV 
						| ImGuiTableFlags_SizingFixedFit 
						| ImGuiTableFlags_SizingFixedSame 
						| ImGuiTableFlags_SizingStretchProp 
						| ImGuiTableFlags_NoPadOuterX;

					ImGui::BeginTable("Transform Table", 2, transfromTableFlags);

					DrawFloat3Component("Translation", component.Translation);

					glm::vec3 rotation = glm::degrees(component.Rotation);
					DrawFloat3Component("Rotation", rotation);
					component.Rotation = glm::radians(rotation);

					// consider of the reset value in Scale
					// maybe the reset value should be following the first value, not any constant value
					DrawFloat3Component("Scale", component.Scale, 1.0f);

					ImGui::EndTable();
				}
			);

			DrawComponent<ColorComponent>("Color", m_SelectedEntity, [](ColorComponent& component)
				{
					ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
				}
			);

			DrawComponent<CameraComponent>("Camera", m_SelectedEntity, [](CameraComponent& component)
				{
					CameraHandler& camera = component.Camera;
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
					else
					{
						float verticalFOV = camera.GetPerspectiveVerticalFOV();
						if (ImGui::DragFloat("FOV", &verticalFOV))
							camera.SetPerspectiveVerticalFOV(verticalFOV);
					}
					//ImGui::DragFloat(sizeLabel,);
					float nearClip = camera.GetCameraNear();
					float farClip = camera.GetCameraFar();

					if (ImGui::DragFloat("Near", &nearClip))
						camera.SetCameraNear(nearClip);

					//ImGui::SameLine();

					if (ImGui::DragFloat("Far", &farClip))
						camera.SetCameraFar(farClip);
				}
			);

			ImGui::TreePop();
		}
	}

}