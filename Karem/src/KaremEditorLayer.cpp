#include "KaremEditorLayer.h"

#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Scene/Components.h"

#include <imgui.h>
#include <imgui_setup.h>

static constexpr int32_t appWidth = 1280, appHeight = 720;

namespace Karem {

	void KaremEditorLayer::OnAttach()
	{
		//------------------------------------------------------------------------
		// this is overriding texture in index 3 in texture renderer
		m_Texture = CreateTexture2D("res/texture/spritesheet/city_tilemap.png", 1);
		m_SpriteSheet = SubTexture2D(m_Texture, { 0,4 }, { 8,8 }, { 5,4 });
		//------------------------------------------------------------------------

		m_Camera = OrthographicCamera({ 16,9 }, 1.0f);
		m_ActiveScene = std::make_shared<Scene>();
		m_HierarcyPanel = SceneHierarcyPanel(m_ActiveScene);

		Entity cameraEntity = m_ActiveScene->CreateEntity("Camera");
		auto& cameraComponent = cameraEntity.AddComponent<CameraComponent>(OrthographicCamera({ 16,9 }, 1));
		auto& camera = cameraComponent.Camera;

		camera.SetPerspectiveCamera(PerspectiveCamera(1.77, 45.0f, 0.01, 100.0f));
		camera.SetToOrthographic();

		Entity SquareEntity = m_ActiveScene->CreateEntity("Square Entity");
		glm::vec4 color = { 0.4f, 0.0f, 1.0f, 1.0f };
		SquareEntity.AddComponent<ColorComponent>(color);
	}

	void KaremEditorLayer::OnDetach()
	{
	}

	void KaremEditorLayer::Update(TimeStep ts)
	{
		//ENGINE_DEBUG("{}", ts);
		//m_Camera.Update(ts);

		m_ActiveScene->m_FrameBuffer->Bind();

		// testing the renderer for texture
		RendererCommand::Clear();
		RendererCommand::ClearColor("#026773");

		static glm::vec4 quadPos = glm::vec4(1.0f);
		static glm::vec2 quadSize = glm::vec2(1.0f);
		static glm::vec4 quadColor = { 0.1f,0.1f,0.5f,1.0f };
		static glm::mat4 quatTransform = glm::mat4(1.0f) * glm::scale(glm::mat4(1.0f), { 5,4,1 });

		//Renderer::BeginScene(m_Camera);
		//Renderer::SubmitQuad(quatTransform, m_SpriteSheet, 1.0f);
		//Renderer::EndScene();


#if OLD_RENDERER
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

		m_ActiveScene->m_FrameBuffer->UnBind();
	}

	void KaremEditorLayer::RenderImGUI()
	{
		m_HierarcyPanel.RenderImGUI();
	}

	void KaremEditorLayer::EventHandler(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&KaremEditorLayer::WindowResizeAction, this, std::placeholders::_1));
		m_ActiveScene->EventHandler(event);
		//m_Camera.EventHandler(event);
	}

	bool KaremEditorLayer::WindowResizeAction(WindowResizeEvent& event)
	{
		// set camera zoom level
		float zoom = (float)event.GetWidth() / (float)event.GetHeight();
		//ENGINE_DEBUG("{}", zoom);
		//m_Camera.SetZoom(zoom);
		return true;
	}

}