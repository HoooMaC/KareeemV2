#include "KaremEditorLayer.h"

#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Scene/Components.h"

#include "Panels/MenuBar.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_setup.h>

static constexpr int32_t appWidth = 1280, appHeight = 720;

namespace Karem {

	void KaremEditorLayer::OnAttach()
	{
		// @
		// ! flkdsfajdslkfjlksdjflksaj
		// ? fkdsjkfsaklfdsjalkj
		// TODO: FDSFFJKJFDLKJAKSFJFKDSAFDJ
		//------------------------------------------------------------------------
		// this is overriding texture in index 3 in texture renderer
		m_Texture = CreateTexture2D("res/texture/spritesheet/city_tilemap.png", 1);
		m_SpriteSheet = SubTexture2D(m_Texture, { 0,4 }, { 8,8 }, { 5,4 });
		//------------------------------------------------------------------------

		m_FrameBuffer = CreateFrameBuffer(1280, 720);
		m_ActiveScene = std::make_shared<Scene>();
		m_HierarcyPanel = SceneHierarcyPanel(m_ActiveScene);

		{
			Entity cameraEntity = m_ActiveScene->CreateEntity("Camera");
			auto& cameraComponent = cameraEntity.AddComponent<CameraComponent>(OrthographicCamera(5.0f, 16.0f / 9.0f, -5.0f, 5.0f));
			auto& cameraHandler = cameraComponent.Camera;
			auto& cameraTransform = cameraEntity.GetComponent<TransformComponent>();
			cameraTransform.Translation.z += 1.1f;
			cameraHandler.SetPerspectiveCamera(PerspectiveCamera(16.0f / 9.0f, glm::radians(45.0f), 0.001f, 1000.0f));
			cameraHandler.SetTypeToOrthographic();
		}

		{
			Entity SquareEntity = m_ActiveScene->CreateEntity("Square Entity");
			auto& squareScale = SquareEntity.GetComponent<TransformComponent>().Scale;
			SquareEntity.AddComponent<ColorComponent>(glm::vec4{ 0.4f, 0.0f, 1.0f, 1.0f });
		}
	}

	void KaremEditorLayer::OnDetach()
	{
	}

	void KaremEditorLayer::Update(TimeStep ts)
	{
		const auto& [fbWidth, fbHeight] = m_FrameBuffer->GetFrameBufferSize();
		if (m_CurrentViewportSize.x != fbWidth or m_CurrentViewportSize.y != fbHeight)
		{
			m_FrameBuffer->Resize((int32_t)m_CurrentViewportSize.x, (int32_t)m_CurrentViewportSize.y);
			m_ActiveScene->OnViewportResize(m_CurrentViewportSize.x, m_CurrentViewportSize.y);
		}

		m_FrameBuffer->Bind();

		// testing the renderer for texture
		RendererCommand::Clear();
		RendererCommand::ClearColor("#1E1E1E");

#if OLD_RENDERER
		static glm::vec4 quadPos = glm::vec4(1.0f);
		static glm::vec2 quadSize = glm::vec2(1.0f);
		static glm::vec4 quadColor = { 0.1f,0.1f,0.5f,1.0f };
		static glm::mat4 quatTransform = glm::mat4(1.0f) * glm::scale(glm::mat4(1.0f), { 5,4,1 });

		//Renderer::BeginScene(m_Camera);
		//Renderer::SubmitQuad(quatTransform, m_SpriteSheet, 1.0f);
		//Renderer::EndScene();
		Renderer2D::BeginScene(m_Camera);
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { -6.0f, 0.0, 0.0f }) * glm::scale(glm::mat4(1.0f), { 10.0f, 10.0f, 1.0f });
		Renderer2D::SubmitSubTexturedQuad({ -1.0f, -1.0f, 0.0f, 1.0f }, { 3.0f, 3.0f }, m_SpriteSheet, 1.0f);
		Renderer2D::SubmitQuad(transform, { 0.3f, 0.0f, 0.9f, 1.0f });
		Renderer2D::SubmitQuad(glm::translate(glm::mat4(1.0f), { 0.0f, -5.0, 0.2f }) * glm::scale(glm::mat4(1.0f), { 3.0f, 3.0f, 1.0f }), m_SpriteSheet, 1.0f);
		for (float i = 0; i < 10; i += 1.0f)
		{
			for (float j = 0; j < 10; j += 1.0f)
			{
				float red = i / 10.0f, green = j / 10.0f;
				const glm::vec4 color = { red, green, 0.5f, 1.0f };
				Renderer2D::SubmitQuad({ i, j, -0.1f, 1.0f }, { 3.0f, 3.0f }, color);
			}
		}
		Renderer2D::EndScene();
#endif

		m_ActiveScene->Update(ts);

		m_FrameBuffer->UnBind();
	}

	void KaremEditorLayer::RenderImGUI()
	{
		imgui::BeginFrame();

		RenderDockspace();
		RenderViewportPanel();

		ImGui::ShowDemoWindow();

		m_HierarcyPanel.RenderImGUI();

		imgui::EndFrame();
	}

	void KaremEditorLayer::EventHandler(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&KaremEditorLayer::WindowResizeAction, this, std::placeholders::_1));
		m_ActiveScene->EventHandler(event);
		//m_Camera.EventHandler(event);
	}

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

	static void MenuBar()
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
			if (ImGui::BeginMenu("Entity"))
			{
				ImGui::MenuItem("Entity List");
				ImGui::MenuItem("Entity Component");
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

	void KaremEditorLayer::RenderDockspace()
	{
#if OLD_VIEWPORT
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGuiWindowFlags docks_window_flags = ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_AlwaysAutoResize
			| ImGuiWindowFlags_NoBringToFrontOnFocus
			| ImGuiWindowFlags_NoNavFocus
			| ImGuiWindowFlags_MenuBar;
			//| ImGuiWindowFlags_NoResize
			//| ImGuiWindowFlags_NoMove

		ImGui::Begin("DockSpaceViewport", NULL, docks_window_flags);

		MenuBar();

		ImGuiID dockspace_id = ImGui::GetID("DockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));
		ImGui::End();

		ImGui::PopStyleVar(3);
#endif
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGuiWindowFlags docks_window_flags = ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_MenuBar;

		ImGui::Begin("DockSpaceViewport", NULL, docks_window_flags);

		MenuBar::Render();

		ImGuiID dockspace_id = ImGui::GetID("DockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

		ImGui::End();

		ImGui::PopStyleVar(3);


	}

	void KaremEditorLayer::RenderViewportPanel()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("Viewport");
		ImVec2 currentPannelSize = ImGui::GetContentRegionAvail();
		m_CurrentViewportSize = *(glm::vec2*)&currentPannelSize;
		uint64_t textureID = m_FrameBuffer->GetTextureColorAttachmentID();
		ImGui::Image(
			reinterpret_cast<void*>(textureID),
			{ (float)m_FrameBuffer->GetFrameBufferSize().width, (float)m_FrameBuffer->GetFrameBufferSize().height },
			ImVec2(0, 1),
			ImVec2(1, 0)
		);

		ImGui::End();

		ImGui::PopStyleVar(3);
	}

	bool KaremEditorLayer::WindowResizeAction(WindowResizeEvent& event)
	{
		return true;
	}

}