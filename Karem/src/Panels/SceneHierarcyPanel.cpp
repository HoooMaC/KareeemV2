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
		//{
		//	auto view = m_ContextScene->m_Registry.view<CameraComponent>();
		//	for (const auto entity : view)
		//	{
		//		auto [component] = view.get(entity);
		//		if (ImGui::Button("ChangeCamera"))
		//		{
		//			auto& Camera = component.Camera;
		//			switch (Camera.GetCurrentCameraType())
		//			{
		//			case CameraType::Orthographic:
		//				Camera.SetCurrrentCamera(CameraType::Perspective);
		//				break;
		//			case CameraType::Perspective:
		//				Camera.SetCurrrentCamera(CameraType::Orthographic);
		//				break;
		//			}
		//		}
		//	}
		//}

		static bool show = true;
		ImGui::ShowDemoWindow(&show);
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
		{	
			//m_SelectedEntity.ShowPropertiesToImGui();

			TagComponent& tag = m_SelectedEntity.GetComponent<TagComponent>();
			TransformComponent& transform = m_SelectedEntity.GetComponent<TransformComponent>();
			ColorComponent& color = m_SelectedEntity.GetComponent<ColorComponent>();

			// Tampilkan komponen-komponen tersebut
			char tagBuffer[256];
			strcpy_s(tagBuffer, tag.Tag.c_str());
			if (ImGui::InputText("##InputText", tagBuffer, IM_ARRAYSIZE(tagBuffer)))
			{
				tag.Tag = tagBuffer;
			}

			ImGui::DragFloat3("Transform", glm::value_ptr(transform.Transform[3]), 0.1f, -10.0f, 20.0f, "%.2f");

			ImGui::ColorEdit4("Color", glm::value_ptr(color.Color));


		}
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

		//if(m_SelectedEntity)
		//{
		//	const char* label = m_SelectedEntity.GetComponent<TagComponent>().Tag.c_str();
		//	ImGui::Text(label);
		//}
		ImGui::End();
	}

}