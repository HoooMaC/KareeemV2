#include "SceneHierarcyPanel.h"

#include "Scene/Components.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_setup.h>

namespace Karem {

	static void ShowExampleMenuFile()
	{
		ImGui::MenuItem("(demo menu)", NULL, false, false);
		if (ImGui::MenuItem("New")) {}
		if (ImGui::MenuItem("Open", "Ctrl+O")) {}
		if (ImGui::BeginMenu("Open Recent"))
		{
			ImGui::MenuItem("fish_hat.c");
			ImGui::MenuItem("fish_hat.inl");
			ImGui::MenuItem("fish_hat.h");
			if (ImGui::BeginMenu("More.."))
			{
				ImGui::MenuItem("Hello");
				ImGui::MenuItem("Sailor");
				if (ImGui::BeginMenu("Recurse.."))
				{
					ShowExampleMenuFile();
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Save", "Ctrl+S")) {}
		if (ImGui::MenuItem("Save As..")) {}

		ImGui::Separator();
		if (ImGui::BeginMenu("Options"))
		{
			static bool enabled = true;
			ImGui::MenuItem("Enabled", "", &enabled);
			ImGui::BeginChild("child", ImVec2(0, 60), true);
			for (int i = 0; i < 10; i++)
				ImGui::Text("Scrolling Text %d", i);
			ImGui::EndChild();
			static float f = 0.5f;
			static int n = 0;
			ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
			ImGui::InputFloat("Input", &f, 0.1f);
			ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Colors"))
		{
			float sz = ImGui::GetTextLineHeight();
			for (int i = 0; i < ImGuiCol_COUNT; i++)
			{
				const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
				ImVec2 p = ImGui::GetCursorScreenPos();
				ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
				ImGui::Dummy(ImVec2(sz, sz));
				ImGui::SameLine();
				ImGui::MenuItem(name);
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Options"))
		{
			static bool b = true;
			ImGui::Checkbox("SomeOption", &b);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Disabled", false)) // Disabled
		{
			IM_ASSERT(0);
		}
		if (ImGui::MenuItem("Checked", NULL, true)) {}
	}

	void SceneHierarcyPanel::RenderImGUI()
	{
		imgui::BeginFrame();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		const ImGuiWindowClass* window_class = NULL;

		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		char label[32];
		imgui::ImFormatString(label, IM_ARRAYSIZE(label), "DockSpaceViewport_%08X", viewport->ID);

		ImGuiWindowFlags host_window_flags = 0;
		host_window_flags |= ImGuiWindowFlags_NoTitleBar;
		host_window_flags |= ImGuiWindowFlags_NoDocking;
		host_window_flags |= ImGuiWindowFlags_NoCollapse;
		host_window_flags |= ImGuiWindowFlags_NoCollapse;
		host_window_flags |= ImGuiWindowFlags_NoResize;
		host_window_flags |= ImGuiWindowFlags_NoMove;
		host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
		host_window_flags |= ImGuiWindowFlags_NoNavFocus;
		host_window_flags |= ImGuiWindowFlags_MenuBar;
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			host_window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::Begin(label, NULL, host_window_flags);

		MenuBar();

		ImGuiID dockspace_id = ImGui::GetID("DockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags, window_class);
		ImGui::End();
	
		ImGui::PopStyleVar(3);
		ViewportPanel();
		EntityListPanel();

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

		ImGui::Begin("Current Properties");

		ImGui::Text("Entities");
		ImGui::Separator();
		{
			auto view = m_ContextScene->m_Registry.view<TagComponent, TransformComponent, ColorComponent>();
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

		imgui::EndFrame();
	}

	void SceneHierarcyPanel::MenuBar()
	{
		// Begin menu bar +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				ShowExampleMenuFile();
				ImGui::Separator();
				ImGui::MenuItem("Quit", "Alt+F4");
				// close window here

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Examples"))
			{
				ImGui::MenuItem("Main menu bar");
				ImGui::MenuItem("Console");
				ImGui::MenuItem("Log");
				ImGui::MenuItem("Simple layout");
				ImGui::MenuItem("Property editor");
				ImGui::MenuItem("Long text display");
				ImGui::MenuItem("Auto-resizing window");
				ImGui::MenuItem("Constrained-resizing window");
				ImGui::MenuItem("Simple overlay");
				ImGui::MenuItem("Fullscreen window");
				ImGui::MenuItem("Manipulating window titles");
				ImGui::MenuItem("Custom rendering");
				ImGui::MenuItem("Dockspace");
				ImGui::MenuItem("Documents");
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Tools"))
			{
				ImGui::MenuItem("Metrics/Debugger");
				ImGui::MenuItem("Debug Log");
				ImGui::MenuItem("Stack Tool");
				ImGui::MenuItem("Style Editor");
				ImGui::MenuItem("About Dear ImGui");
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		// End menu bar +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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
			void* camera = activeCamera->GetCamera(CameraType::Orthographic);
			auto type = activeCamera->GetCurrentCameraType();
			if (type == CameraType::Orthographic)
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
		if (m_SelectionContext)
		{	

		}
	}

	void SceneHierarcyPanel::EntityListPanel()
	{
		ImGui::Begin("Entity List");
		m_ContextScene->m_Registry.view<TagComponent>().each([](auto entity, TagComponent& tag) {
			const std::string& tagName = tag.Tag;
			ImGui::Text("Entity: %s", tagName.c_str());
		});
		ImGui::End();
	}

}