#include "KaremEngine.h"

#include "SceneHierarcyPanel.h"

#include "Platform/Utils/FileDialog.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <filesystem>

namespace Karem {

	void SceneHierarcyPanel::RenderEntityList(Entity& selectedEntity, Entity& mainCamera, const std::shared_ptr<Scene>& m_ContextScene, bool* panelPointer)
	{

		ImGui::Begin("Entity List", panelPointer);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f,0.0f });
		m_ContextScene->m_Registry.view<TagComponent>().each([&](entt::entity entityId, TagComponent& tagComponent)
			{
				Entity entity = Entity{ entityId, m_ContextScene.get() };

				ImGuiTreeNodeFlags entityListFlags = selectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0;
				entityListFlags |= ImGuiTreeNodeFlags_Framed
					| ImGuiTreeNodeFlags_OpenOnArrow
					| ImGuiTreeNodeFlags_SpanAvailWidth;

				const std::string& tag = tagComponent.Tag;
				bool isTreeOpened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, entityListFlags, tag.c_str());

				if (ImGui::IsItemClicked())
				{
					selectedEntity = entity;
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
					if (selectedEntity == entity)
						selectedEntity = {};
					if (mainCamera == entity)
						mainCamera = {};
				}
			});

		if (ImGui::IsMouseDown(0) and ImGui::IsWindowHovered())
			selectedEntity = {};

		if (ImGui::BeginPopupContextWindow("Create new entity", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_NoOpenOverExistingPopup))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_ContextScene->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}
		ImGui::PopStyleVar();
		ImGui::End();
	}

	void SceneHierarcyPanel::RenderEntityComponent(Entity& selectedEntity, Entity& mainCamera, bool* panelPointer)
	{
		ImGui::Begin("Selected Entity Properties", panelPointer);

		if (selectedEntity)
			DrawEntityComponents(selectedEntity, mainCamera);

		ImGui::End();
	}

	void SceneHierarcyPanel::RenderCameraPanel(Entity& mainCamera, const std::shared_ptr<Scene>& scene, bool* panelPointer)
	{
		ImGui::Begin("Camera Entity", panelPointer);

		bool currentCameraSelected = false;

		auto view = scene->m_Registry.view<CameraComponent>();

		for (const auto& entity : view)
		{
			auto [camera] = view.get(entity);
			if (camera.MainCamera)
				mainCamera = { entity , scene.get() };
			break;
		}

		const char* combopreview = mainCamera ? mainCamera.GetComponent<TagComponent>().Tag.c_str() : "No Main Camera";

		ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());
		if (ImGui::BeginCombo("##Camera", combopreview))
		{
			scene->m_Registry.view<TagComponent, CameraComponent>().each([&](entt::entity entityId, TagComponent& tagComponent, CameraComponent& cameraComponent)
				{
					currentCameraSelected = mainCamera == entityId;

					if (ImGui::Selectable(tagComponent.Tag.c_str(), currentCameraSelected))
					{
						cameraComponent.MainCamera = true;
						if (mainCamera and mainCamera != entityId)
							mainCamera.GetComponent<CameraComponent>().MainCamera = false;
						mainCamera = { entityId, scene.get() };
					}
				});

			ImGui::EndCombo();
		}
		ImGui::End();
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
			//ImGui::PushStyleColor(ImGuiCol_HeaderHovered, HexToVec4<ImVec4>(Color::Header));

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
			const auto contentRegionAvail = ImGui::GetContentRegionAvail();
			const auto fontSize = ImGui::GetFontSize();
			const auto& style = ImGui::GetStyle();
			const float lineHeight = fontSize + style.FramePadding.y * 2.0f;
			ImVec2 buttonSize = { lineHeight, lineHeight };

			bool componentOpened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, label.data());

			//ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

			std::string popupLabel = "RemoveComponent";
			popupLabel.insert(6, label.data());

			if (isRemovable)
			{
				ImGui::SameLine(contentRegionAvail.x - lineHeight);
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

	void SceneHierarcyPanel::DrawEntityComponents(Entity& selectedEntity, Entity& mainCamera)
	{
		static constexpr ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen
			| ImGuiTreeNodeFlags_Framed
			| ImGuiTreeNodeFlags_FramePadding
			| ImGuiTreeNodeFlags_OpenOnArrow
			| ImGuiTreeNodeFlags_OpenOnDoubleClick
			| ImGuiTreeNodeFlags_AllowItemOverlap
			| ImGuiTreeNodeFlags_SpanAvailWidth;

		const auto contentRegionAvail = ImGui::GetContentRegionMax();

		const auto fontSize = ImGui::GetFontSize();
		const auto& style = ImGui::GetStyle();
		const float lineHeight = fontSize + style.FramePadding.y * 2.0f;
		const ImVec2 buttonSize = { lineHeight, lineHeight };
		//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 4,4 });
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 4,4 });
		std::string& tag = selectedEntity.GetComponent<TagComponent>().Tag;

		ImGui::Separator();
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)selectedEntity, flags, tag.c_str());
		ImGui::PopStyleVar();
		
		ImGui::SameLine((contentRegionAvail.x - (buttonSize.x / 2)) - 8.0f); //TODO::Need to fix 8.0f here
		if (ImGui::Button("+", buttonSize))
			ImGui::OpenPopup("Add Component");

		if (ImGui::BeginPopup("Add Component", ImGuiWindowFlags_NoMove))
		{
			if (!selectedEntity.IsHasComponent<TransformComponent>())
			{
				if (ImGui::MenuItem("Transform"))
				{
					selectedEntity.AddComponent<TransformComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!selectedEntity.IsHasComponent<SpriteRendererComponent>())
			{
				if (ImGui::MenuItem("Color"))
				{
					selectedEntity.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!selectedEntity.IsHasComponent<CameraComponent>())
			{
				if (ImGui::MenuItem("Camera"))
				{
					selectedEntity.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::EndPopup();
		}

		uint64_t id = selectedEntity.GetUUID();
		std::string text = "ID : " + std::to_string(id);

		ImGui::PushStyleColor(ImGuiCol_Text, HexToVec4<ImVec4>(Color::White, 0.7f));
		ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMin().x);
		ImGui::Text(text.c_str());
		ImGui::PopStyleColor();


		if (opened)
		{
			ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

			DrawComponent<TagComponent>("Tag", selectedEntity, [&](TagComponent& component)
			{		
				std::string& tag = selectedEntity.GetComponent<TagComponent>().Tag;
				const char* tc = tag.c_str();

				char tagBuffer[256];
				strcpy_s(tagBuffer, sizeof(tagBuffer), tc);
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
				auto regionAvail = ImGui::GetContentRegionAvail();
				ImGui::SetNextItemWidth(regionAvail.x);
				if (ImGui::InputText("##InputText", tagBuffer, IM_ARRAYSIZE(tagBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
					tag = tagBuffer;

				ImGui::PopStyleVar();
			}, false); // TODO : The argument false here is very weird. Think to move into the component itself

			DrawComponent<TransformComponent>("Transform", selectedEntity, [](TransformComponent& component)
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
			});

			DrawComponent<SpriteRendererComponent>("Color", selectedEntity, [](SpriteRendererComponent& component)
			{
				ImGui::ColorEdit4("##Color", glm::value_ptr(component.Color));

				ImGui::PushStyleColor(ImGuiCol_Button, HexToVec4<ImVec4>(Color::Header, 0.4f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HexToVec4<ImVec4>(Color::Header, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, HexToVec4<ImVec4>(Color::Header, 0.8f));

				ImVec2 buttonSize = { ImGui::GetContentRegionMax().x, 25.0f };
				std::string buttonTitle = "Texture";

				std::filesystem::path buttonPath;
				bool isHasTexture = component.Texture != nullptr;
				if (isHasTexture)
				{
					buttonPath = std::filesystem::path(component.Texture->GetFilePath());
					buttonTitle.append(" : " + buttonPath.filename().string());
					buttonSize.x -= 62.5f;
				}

				ImGui::Button(buttonTitle.c_str(), buttonSize);
				if(ImGui::IsItemHovered() and ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					const std::string texturePath = FileDialog::OpenFile("png\0*.png\0");
					if(!texturePath.empty())
					{
						//TODO::Concern about the path here, maybe we can use popup window. Whatever
						std::shared_ptr<Texture2D> newTexture = CreateTexture2D(texturePath, 3);
						component.Texture = newTexture;
					}
				}

				ImGui::SameLine();

				if(isHasTexture)
				{
					ImGui::Button("-", { 25.0f, 25.0f });
					if (ImGui::IsItemHovered() and ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					{
						// TODO::Later, maybe we can add some validation with popup window
						component.Texture = nullptr;
					}
				}

				ImGui::PopStyleColor(3);
			});

			DrawComponent<CameraComponent>("Camera", selectedEntity, [](CameraComponent& component)
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
			}, selectedEntity == mainCamera ? false : true ); // TODO: THIS IS WEIRD

			ImGui::TreePop();
		}
	}

}