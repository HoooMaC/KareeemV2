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
		fbSpecs.Attachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::RED_INTEGER, FrameBufferTextureFormat::DEPTH24STENCIL8 };
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
		//if (Karem::IsImGuiContextValid())
		//{
		//	if (auto viewportWindow = ImGui::FindWindowByName("Viewport"))
		//	{
		//		m_CurrentViewportSize = *(glm::vec2*)&viewportWindow->Size;
		//		m_CurrentViewportSize.y -= viewportWindow->TitleBarHeight();
		//	}
		//}

		if (m_CurrentViewportSize != m_PreviousViewportSize and (m_CurrentViewportSize.x > 0 and m_CurrentViewportSize.y > 0))
		{
			m_PreviousViewportSize = m_CurrentViewportSize;
			m_FrameBuffer->Resize((int32_t)m_CurrentViewportSize.x, (int32_t)m_CurrentViewportSize.y);
			m_ActiveScene->OnViewportResize((float)m_CurrentViewportSize.x, (float)m_CurrentViewportSize.y);
			m_EditorCamera.SetViewportSize((float)m_CurrentViewportSize.x, (float)m_CurrentViewportSize.y);
		}

		m_FrameBuffer->Bind();

		// testing the renderer for texture
		RendererCommand::ClearColor(Color::ViewportBackground);
		RendererCommand::Clear();

		m_FrameBuffer->ClearColorAttachment(1, -1);

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
		m_BrowserPanel.Render();

		imgui::EndFrame();
	}

	void KaremEditorLayer::EventHandler(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&KaremEditorLayer::MouseButtonAction, this, std::placeholders::_1));

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
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGuiWindowFlags docks_window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar;

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

		auto currentViewportSize = ImGui::GetContentRegionAvail();
		m_CurrentViewportSize = *(glm::vec2*)&currentViewportSize;

		float fbWidth = m_FrameBuffer->GetFramebufferWidth();
		float fbHeight = m_FrameBuffer->GetFramebufferHeight();
		uint64_t textureID = m_FrameBuffer->GetTextureColorAttachmentID(0);

		ImGui::Image(
			reinterpret_cast<void*>(textureID),
			{ fbWidth, fbHeight },
			ImVec2(0, 1),
			ImVec2(1, 0)
		);

		// TODO::Check this if we already have play state
		if(m_CurrentState == State::Edit)
		{
			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM");
				if (payload)
				{
					const char* receivedData = (const char*)payload->Data;
					OpenScene(receivedData);
				}

				ImGui::EndDragDropTarget();
			}
		}

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

			bool manipulated = ImGuizmo::Manipulate(
				glm::value_ptr(viewMatrix), 
				glm::value_ptr(projMatrix),
				(ImGuizmo::OPERATION)m_CurrentGizmoType,
				ImGuizmo::MODE::LOCAL,
				glm::value_ptr(transform), //MATRIX
				nullptr // DELTA MATRIX
			);

			if (manipulated)
			{
				glm::vec3 translation = glm::vec3(1.0f), rotation = glm::vec3(1.0f), scale = glm::vec3(1.0f);

				//ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(translation), glm::value_ptr(rotation), glm::value_ptr(scale));
				if(Math::DecomposeMatrix(transform, translation, rotation, scale))
				{
					tc.Translation = translation;

					// TODO::FIX ROTATION
					glm::vec3 deltaRotation = rotation - tc.Rotation;
					tc.Rotation += deltaRotation;

					tc.Scale = scale;
				}
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
		return false;
	}

	bool Karem::KaremEditorLayer::MouseButtonAction(MouseButtonPressedEvent& event)
	{
		if (event.GetMouseButton() == Mouse::ButtonLeft)
		{
			// Get the viewport 
			// TODO::m_Viewport.GetSize()
			// TODO::m_Viewport.GetMinBounds()
			const auto viewport = ImGui::FindWindowByName("Viewport");
			const auto viewportRect = viewport->InnerRect;
			const auto [viewportSizeX, viewportSizeY] = viewportRect.GetSize();

			auto [mx, my] = ImGui::GetMousePos();

			my -= viewportRect.Min.y;
			mx -= viewportRect.Min.x;
			my = viewportSizeY - my;

			if (mx > 0 and my > 0 and mx < viewportSizeX and my < viewportSizeY)
			{
				m_FrameBuffer->Bind();
				int selectedEntity = m_FrameBuffer->GetEntityId(1, mx, my);
				m_FrameBuffer->UnBind();

				//ENGINE_DEBUG("Entity Id {}", selectedEntity);
				if(!ImGuizmo::IsOver())
				{
					if (selectedEntity != -1)
						m_Panels.SetSelectedEntity(selectedEntity);
					else
						m_Panels.SetSelectedEntity();
				}
			}
		}
		return false;
	}

}