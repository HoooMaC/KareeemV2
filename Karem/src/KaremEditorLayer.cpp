#include "KaremEditorLayer.h"

#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Scene/Components.h"
#include "Scene/SceneSerializer.h"

#include "Math/Matrix.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_setup.h>
#include "external/imgui/imgui_configuration.h"

#include "external/ImGuizmo/ImGuizmo.h"

static constexpr int32_t appWidth = 1280, appHeight = 720;

namespace Karem {

	void KaremEditorLayer::OnAttach()
	{
		// @
		// ! flkdsfajdslkfjlksdjflksaj
		// ? fkdsjkfsaklfdsjalkj
		// TODO: FDSFFJKJFDLKJAKSFJFKDSAFDJ
		//------------------------------------------------------------------------
		//m_Texture = CreateTexture2D("res/texture/spritesheet/city_tilemap.png", 1);
		//m_SpriteSheet = SubTexture2D(m_Texture, { 0,4 }, { 8,8 }, { 5,4 });
		//------------------------------------------------------------------------
		m_EditorCamera = EditorCamera(45.f, 1.777f, 0.0001f, 1000.f);

		FrameBufferSpecifications fbSpecs;
		fbSpecs.Width = 1280;
		fbSpecs.Height = 720;
		fbSpecs.Attachments = { FrameBufferTextureFormat::RGBA8,FrameBufferTextureFormat::DEPTH24STENCIL8 };
		m_FrameBuffer = CreateFrameBuffer(fbSpecs);

		m_ActiveScene = std::make_shared<Scene>();
		m_Panels.SetScene(m_ActiveScene);
		m_Panels.SetNewSceneFuction(std::bind(&KaremEditorLayer::NewScene, this));
		m_Panels.SetSaveSceneAsFuction(std::bind(&KaremEditorLayer::SaveSceneAs, this, std::placeholders::_1));
		m_Panels.SetOpenSceneFuction(std::bind(&KaremEditorLayer::OpenScene, this, std::placeholders::_1));
#if 0
		{
			Entity cameraEntity = m_ActiveScene->CreateEntity("Camera");
			auto& cameraComponent = cameraEntity.AddComponent<CameraComponent>(OrthographicCamera(5.0f, 16.0f / 9.0f, -50.0f, 50.0f));
			auto& cameraHandler = cameraComponent.Camera;
			cameraComponent.MainCamera = true;
			cameraHandler.SetPerspectiveCamera(PerspectiveCamera(16.0f / 9.0f, glm::radians(45.0f), 0.001f, 1000.0f));
			cameraHandler.SetTypeToOrthographic();
		}

		{
			Entity SquareEntity = m_ActiveScene->CreateEntity("Square Entity");
			auto& squareScale = SquareEntity.GetComponent<TransformComponent>().Scale;
			SquareEntity.AddComponent<ColorComponent>(glm::vec4{ 0.4f, 0.0f, 1.0f, 1.0f });
	}
#endif
}

	void KaremEditorLayer::OnDetach()
	{
	}

	void KaremEditorLayer::Update(TimeStep ts)
	{
		float titleBarHeight = 0.0f;
		if (Karem::IsImGuiContextValid())
		{
			if (auto viewportWindow = ImGui::FindWindowByName("Viewport"))
			{
				m_CurrentViewportSize = { viewportWindow->Size.x, viewportWindow->Size.y };
				titleBarHeight = viewportWindow->TitleBarHeight();
			}
		}

		if (m_CurrentViewportSize != m_PreviousViewportSize and (m_CurrentViewportSize.x > 0 and m_CurrentViewportSize.y > 0))
		{
			m_PreviousViewportSize = m_CurrentViewportSize;
			m_CurrentViewportSize.y -= titleBarHeight;
			m_FrameBuffer->Resize((int32_t)m_CurrentViewportSize.x, (int32_t)m_CurrentViewportSize.y);
			m_ActiveScene->OnViewportResize((float)m_CurrentViewportSize.x, (float)m_CurrentViewportSize.y);
			m_EditorCamera.SetViewportSize((float)m_CurrentViewportSize.x, (float)m_CurrentViewportSize.y);
		}

		m_FrameBuffer->Bind();

		// testing the renderer for texture
		RendererCommand::Clear();
		RendererCommand::ClearColor(Color::ViewportBackground);

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
		switch (m_CurrentState)
		{
			case State::Edit:
			{
				m_ActiveScene->UpdateOnEdit(ts, m_EditorCamera);
				m_EditorCamera.OnUpdate(ts);
				break;
			}
			case State::Play:
			{
				m_ActiveScene->UpdateOnPlay(ts);
				break;
			}
		}

		m_FrameBuffer->UnBind();
	}

	void KaremEditorLayer::RenderImGUI()
	{
		imgui::BeginFrame();

		RenderDockspace();
		RenderViewportPanel();

		ImGui::ShowDemoWindow();

		m_Panels.RenderHierarcyPanel();

		imgui::EndFrame();
	}

	void KaremEditorLayer::EventHandler(Event& event)
	{
		EventDispatcher dispatcher(event);

		m_ActiveScene->EventHandler(event);
		m_Panels.EventHandler(event);

		switch (m_CurrentState)
		{
			case State::Edit:
			{
				m_EditorCamera.EventHandler(event);
				break;
			}
			case State::Play:
			{
				break;
			}
		}
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

		m_Panels.RenderMenubar();

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
		float fbWidth = m_FrameBuffer->GetFramebufferWidth();
		float fbHeight = m_FrameBuffer->GetFramebufferHeight();
		uint64_t textureID = m_FrameBuffer->GetTextureColorAttachmentID();
		ImGui::Image(
			reinterpret_cast<void*>(textureID),
			{ fbWidth, fbHeight },
			ImVec2(0, 1),
			ImVec2(1, 0)
		);

		// Gizmos Here

		if (ImGui::IsWindowHovered())
		{
			if (Input::IsKeyPressed(Key::V))
			{
				m_CurrentGizmoType = -1;
			}
			else if (Input::IsKeyPressed(Key::S))
			{
				m_CurrentGizmoType = ImGuizmo::OPERATION::SCALE;
			}
			// IN PROCESSSSSS
			else if (Input::IsKeyPressed(Key::R))
			{
				m_CurrentGizmoType = ImGuizmo::OPERATION::ROTATE;
			}
			else if (Input::IsKeyPressed(Key::E)) // i dont know what the good keyword for this
			{
				m_CurrentGizmoType = ImGuizmo::OPERATION::TRANSLATE;
			}
		}

		Entity& selectedEntity = m_Panels.GetSelectedEntity();
		if (selectedEntity and m_CurrentGizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			const auto& windowPos = ImGui::GetWindowPos();
			const auto& windowSize = ImGui::GetWindowSize();
			ImGuizmo::SetRect(windowPos.x, windowPos.y, windowSize.x, windowSize.y);

			auto& entityTranslation = selectedEntity.GetComponent<TransformComponent>().Translation;
			auto& entityRotation = selectedEntity.GetComponent<TransformComponent>().Rotation;
			auto& entityScale = selectedEntity.GetComponent<TransformComponent>().Scale;

			auto& tc = selectedEntity.GetComponent<TransformComponent>();

			glm::mat4 transform = tc.GetTransformMatrix();
			glm::mat4 viewMatrix = m_EditorCamera.GetViewMatrix();
			glm::mat4 projMatrix = m_EditorCamera.GetProjectionMatrix();

			ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projMatrix), (ImGuizmo::OPERATION)m_CurrentGizmoType, ImGuizmo::MODE::LOCAL, glm::value_ptr(transform), nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;

				Math::DecomposeMatrix(transform, translation, rotation, scale);

				tc.Translation = translation;

				// TODO::FIX ROTATION
				//glm::vec3 deltaRotation = rotation - tc.Rotation;
				//tc.Rotation += deltaRotation;

				tc.Scale = scale;
			}

		}
		ImGui::End();

		ImGui::PopStyleVar(3);
	}

	bool KaremEditorLayer::WindowResizeAction(WindowResizeEvent& event)
	{
		return true;
	}

	bool KaremEditorLayer::KeyPressedAction(KeyPressedEvent& event)
	{
		event.GetKeyCode();
		return false;
	}

}