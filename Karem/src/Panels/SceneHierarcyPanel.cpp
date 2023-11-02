#include "SceneHierarcyPanel.h"

#include "Scene/Components.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "MenuBar.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Karem {

	void SceneHierarcyPanel::RenderImGUI()
	{
		if (MenuBar::showEntityList)
		{
			ImGui::Begin("Entity List", &MenuBar::showEntityList);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f,0.0f });
			m_ContextScene->m_Registry.view<TagComponent>().each([&](entt::entity entityId, TagComponent& tagComponent)
				{
					Entity entity = Entity{ entityId, m_ContextScene.get() };

					ImGuiTreeNodeFlags entityListFlags = m_SelectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0;
					entityListFlags |= ImGuiTreeNodeFlags_Framed
						| ImGuiTreeNodeFlags_OpenOnArrow
						| ImGuiTreeNodeFlags_SpanAvailWidth;

					const std::string& tag = tagComponent.Tag;
					bool isTreeOpened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, entityListFlags, tag.c_str());

					if (ImGui::IsItemClicked())
					{
						m_SelectedEntity = entity;
					}

					bool isEntityDeleted = false;
					if (ImGui::BeginPopupContextItem())
					{
						if (ImGui::MenuItem("Delete Entity"))
						{
							isEntityDeleted = true;
						}
						ImGui::EndPopup();
					}

					if (isTreeOpened)
					{
						ImGui::TreePop();
					}

					if (isEntityDeleted)
					{
						m_ContextScene->DestroyEntity(entity);
						if (m_SelectedEntity == entity)
							m_SelectedEntity = {};
						if (m_MainCamera == entity)
							m_MainCamera = {};
					}
				});

			if (ImGui::IsMouseDown(0) and ImGui::IsWindowHovered())
				m_SelectedEntity = {};

			if (ImGui::BeginPopupContextWindow("Create new entity", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_NoOpenOverExistingPopup))
			{
				if (ImGui::MenuItem("Create Empty Entity"))
					m_ContextScene->CreateEntity("Empty Entity");

				ImGui::EndPopup();
			}
			ImGui::PopStyleVar();
			ImGui::End();
		}


		if (MenuBar::showEntityComponent)
		{
			ImGui::Begin("Selected Entity Properties", &MenuBar::showEntityComponent);

			if (m_SelectedEntity)
				DrawEntityComponents(m_SelectedEntity);

			ImGui::End();
		}

		if (MenuBar::showCameraPanel)
		{
			ImGui::Begin("Camera Entity", &MenuBar::showCameraPanel);

			bool currentCameraSelected = false;

			auto view = m_ContextScene->m_Registry.view<CameraComponent>();

			for (const auto& entity : view)
			{
				auto [camera] = view.get(entity);
				if (camera.MainCamera)
					m_MainCamera = { entity , m_ContextScene.get() };
				break;
			}

			const char* combopreview = m_MainCamera ? m_MainCamera.GetComponent<TagComponent>().Tag.c_str() : "No Main Camera";

			if (ImGui::BeginCombo("##Camera", combopreview))
			{
				m_ContextScene->m_Registry.view<TagComponent, CameraComponent>().each([&](entt::entity entityId, TagComponent& tagComponent, CameraComponent& cameraComponent)
				{
					currentCameraSelected = m_MainCamera == entityId;

					if (ImGui::Selectable(tagComponent.Tag.c_str(), currentCameraSelected))
					{
						cameraComponent.MainCamera = true;
						if(m_MainCamera and m_MainCamera != entityId)
							m_MainCamera.GetComponent<CameraComponent>().MainCamera = false;
						m_MainCamera = { entityId, m_ContextScene.get() };
					}
				});

				ImGui::EndCombo();
			}
			ImGui::End();
		}

	}
	void SceneHierarcyPanel::DrawEntityTree(Entity entity, TagComponent& tag)
	{
		static constexpr ImGuiTreeNodeFlags flags = //ImGuiTreeNodeFlags_Framed
			//| ImGuiTreeNodeFlags_FramePadding
			ImGuiTreeNodeFlags_AllowItemOverlap
			| ImGuiTreeNodeFlags_SpanAvailWidth;

		const auto contentRegionAvail = ImGui::GetContentRegionAvail();
		const auto fontSize = ImGui::GetFontSize();
		const auto& style = ImGui::GetStyle();
		const float lineHeight = fontSize + style.FramePadding.y * 2.0f;
		const ImVec2 buttonSize = { lineHeight, lineHeight };

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags | ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0), tag.Tag.c_str());
		//opened &= (bool)m_SelectedEntity;

		ImGui::SameLine(contentRegionAvail.x - buttonSize.x * 0.5f);

		const std::string popupLabel = "Delete Entity" + tag.Tag;

		if (ImGui::Button("+", buttonSize))
			ImGui::OpenPopup(popupLabel.c_str());

		if (ImGui::IsMouseDown(1) and ImGui::IsItemHovered())
		{
			ImGui::OpenPopup(popupLabel.c_str());
		}

		bool isEntityDestroyed = false;
		if (ImGui::BeginPopup(popupLabel.c_str(), ImGuiWindowFlags_NoMove | ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_NoOpenOverExistingPopup))
		{
			if (ImGui::MenuItem("Remove Entity"))
			{
				isEntityDestroyed = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		//ImGui::TreePushOverrideID(id);
		if (opened)
		{
			m_SelectedEntity = { entity, m_ContextScene.get() };
			ImGui::TreePop();
		}

		if (isEntityDestroyed)
		{
			m_ContextScene->DestroyEntity(entity);
			if (m_SelectedEntity == entity)
				m_SelectedEntity = {};
			if (m_MainCamera == entity)
				m_MainCamera = {};
		}
	}

	static void DrawFloat3Component(const char* label, glm::vec3& values, float resetValue = 0.0, float columnWidth = 100.0f)
	{
		auto* g = ImGui::GetCurrentContext();
		ImGuiTable* table = g->CurrentTable;
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
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

			ImGui::PushStyleColor(ImGuiCol_Button, HexToVec4<ImVec4>(Color::RedButton));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HexToVec4<ImVec4>(Color::RedButtonHovered));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, HexToVec4<ImVec4>(Color::RedButtonActive));
			if (ImGui::Button("X", buttonSize))
				values.x = resetValue;
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, HexToVec4<ImVec4>(Color::GreenButton));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HexToVec4<ImVec4>(Color::GreenButtonHovered));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, HexToVec4<ImVec4>(Color::GreenButtonActive));
			if (ImGui::Button("Y", buttonSize))
				values.y = resetValue;
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, HexToVec4<ImVec4>(Color::BlueButton));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HexToVec4<ImVec4>(Color::BlueButtonHovered));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, HexToVec4<ImVec4>(Color::BlueButtonActive));
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
		ImGui::PopStyleVar();
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(std::string_view label, Entity entity, UIFunction uiFunction, bool isRemovable = true)
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

			bool componentOpened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, label.data());
			ImGui::PopStyleVar();

			std::string popupLabel = "RemoveComponent";
			popupLabel.insert(6, label.data());

			if(isRemovable)
			{
				ImGui::SameLine(contentRegionAvail.x + lineHeight / 2.0f);
				if (ImGui::Button("-", buttonSize))
					ImGui::OpenPopup(popupLabel.c_str());
			}

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

	void SceneHierarcyPanel::DrawEntityComponents(Entity entity)
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

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 4,4 });
		std::string& tag = m_SelectedEntity.GetComponent<TagComponent>().Tag;

		ImGui::Separator();
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		ImGui::PopStyleVar();

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

		if (opened)
		{

			DrawComponent<TagComponent>("Tag", m_SelectedEntity, [&](TagComponent& component)
				{
					std::string& tag = m_SelectedEntity.GetComponent<TagComponent>().Tag;
					const char* tc = tag.c_str();

					char tagBuffer[256];
					strcpy_s(tagBuffer, sizeof(tagBuffer), tc);
					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
					auto regionAvail = ImGui::GetContentRegionAvail();
					ImGui::SetNextItemWidth(regionAvail.x);
					if (ImGui::InputText("##InputText", tagBuffer, IM_ARRAYSIZE(tagBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
						tag = tagBuffer;

					ImGui::PopStyleVar();
				}, false ); // TODO : The argument false here is very weird. Think to move into the component itself

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

					// TODO: consider of the reset value in Scale
					// TODO: maybe the reset value should be following the first value, not any constant value
					DrawFloat3Component("Scale", component.Scale, 1.0f);

					ImGui::EndTable();
				}
			);

			DrawComponent<ColorComponent>("Color", m_SelectedEntity, [](ColorComponent& component)
				{
					ImGui::ColorEdit4("##Color", glm::value_ptr(component.Color));
				}
			);

			DrawComponent<CameraComponent>("Camera", m_SelectedEntity, [](CameraComponent& component)
				{
					static constexpr ImGuiTableFlags cameraTableFlags = ImGuiTableFlags_BordersInnerV
						| ImGuiTableFlags_SizingFixedFit
						| ImGuiTableFlags_SizingFixedSame
						| ImGuiTableFlags_SizingStretchProp
						| ImGuiTableFlags_NoPadOuterX;

					CameraHandler& camera = component.Camera;
					ImGui::BeginTable("Camera Table", 2, cameraTableFlags);

					ImGui::TableNextRow();

					ImGui::TableSetColumnIndex(0);
					ImGui::Text("Projection");
					ImGui::TableSetColumnIndex(1);
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0,0 });

					auto regionAvail = ImGui::GetContentRegionAvail();
					ImGui::SetNextItemWidth(regionAvail.x);
					if (ImGui::BeginCombo("##Projection", camera.GetCameraStringType()))
					{
						bool isOrthographicSelected = "Orthographic" == camera.GetCameraStringType() ? true : false;
						bool isPerspectiveSelected = isOrthographicSelected ^ 1;
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

					ImGui::TableNextRow();

					ImGui::TableSetColumnIndex(0);
					if (camera.IsOrthographic())
					{
						ImGui::Text("Size");
						ImGui::TableSetColumnIndex(1);

						float orthoSize = camera.GetOrthographicSize();
						regionAvail = ImGui::GetContentRegionAvail();
						ImGui::SetNextItemWidth(regionAvail.x);
						if (ImGui::DragFloat("##OrthoSize", &orthoSize))
							camera.SetOrthographicSize(orthoSize);
					}
					else
					{
						ImGui::Text("FOV");
						ImGui::TableSetColumnIndex(1);

						float verticalFOV = camera.GetPerspectiveVerticalFOV();
						regionAvail = ImGui::GetContentRegionAvail();
						ImGui::SetNextItemWidth(regionAvail.x);
						if (ImGui::DragFloat("##FOV", &verticalFOV))
							camera.SetPerspectiveVerticalFOV(verticalFOV);
					}

					ImGui::TableNextRow();

					ImGui::TableSetColumnIndex(0);
					ImGui::Text("Near");
					ImGui::TableSetColumnIndex(1);
					
					float nearClip = camera.GetCameraNear();
					regionAvail = ImGui::GetContentRegionAvail();
					ImGui::SetNextItemWidth(regionAvail.x);
					if (ImGui::DragFloat("##Near", &nearClip))
						camera.SetCameraNear(nearClip);

					ImGui::TableNextRow();

					ImGui::TableSetColumnIndex(0);
					ImGui::Text("Far");
					ImGui::TableSetColumnIndex(1);
					

					float farClip = camera.GetCameraFar();
					regionAvail = ImGui::GetContentRegionAvail();
					ImGui::SetNextItemWidth(regionAvail.x);
					if (ImGui::DragFloat("##Far", &farClip))
						camera.SetCameraFar(farClip);

					ImGui::PopStyleVar();
					ImGui::EndTable();
				}
			);

			ImGui::TreePop();
		}
	}

}