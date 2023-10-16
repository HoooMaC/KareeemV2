#include "SandboxLayer.h"

#include "Scene/Scene.h"
#include "Scene/Entity.h"

#include "imgui.h"
#include "imgui_setup.h"

static constexpr int32_t appWidth = 1280, appHeight = 720;

void SandboxLayer::OnAttach()
{
	// this is overriding texture in index 3 in texture renderer
	m_Texture = Karem::CreateTexture2D("res/texture/spritesheet/city_tilemap.png", 1);
	m_SpriteSheet = Karem::SubTexture2D(m_Texture, { 0,4 }, { 8,8 }, { 5,4 });
	m_FrameBuffer = Karem::CreateFrameBuffer(1280, 720);
	m_Camera = Karem::OrthographicCamera({ 16,9 }, 1.0f);

	Karem::Entity cameraEntity = m_ActiveScene.CreateEntity("Camera");
	//cameraEntity.AddComponent<Karem::CameraComponent>(Karem::PerspectiveCamera(1280/720, glm::radians(45.0f), 0.001, 1000.0f));

	auto camera = cameraEntity.AddComponent<Karem::CameraComponent>(Karem::OrthographicCamera({16,9},1)).Camera;
	camera.SetPerspectiveCamera(Karem::PerspectiveCamera(1280 / 720, glm::radians(45.0f), 0.001, 1000.0f));
	camera.SetCurrrentCamera(Karem::CameraType::Orthographic);

	Karem::Entity SquareEntity = m_ActiveScene.CreateEntity("Square Entity");
	glm::vec4 color = { 0.4f, 0.0f, 1.0f, 1.0f };
	SquareEntity.AddComponent<Karem::ColorComponent>(color);
}

void SandboxLayer::OnDetach()
{
}

void SandboxLayer::Update(Karem::TimeStep ts)
{
	//ENGINE_DEBUG("{}", ts);
	//m_Camera.Update(ts);

	m_FrameBuffer->Bind();

	// testing the renderer for texture
	Karem::RendererCommand::Clear();
	Karem::RendererCommand::ClearColor("#026773");

	static glm::vec4 quadPos = glm::vec4(1.0f);
	static glm::vec2 quadSize = glm::vec2(1.0f);
	static glm::vec4 quadColor = { 0.1f,0.1f,0.5f,1.0f };
	static glm::mat4 quatTransform = glm::mat4(1.0f) * glm::scale(glm::mat4(1.0f), { 5,4,1 });

	//Karem::Renderer::BeginScene(m_Camera);
	//Karem::Renderer::SubmitQuad(quatTransform, m_SpriteSheet, 1.0f);
	//Karem::Renderer::EndScene();


#if OLD_RENDERER
	Karem::Renderer2D::BeginScene(m_Camera);
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), { -6.0f, 0.0, 0.0f }) * glm::scale(glm::mat4(1.0f), { 10.0f, 10.0f, 1.0f });
	Karem::Renderer2D::SubmitSubTexturedQuad({ -1.0f, -1.0f, 0.0f, 1.0f }, { 3.0f, 3.0f }, m_SpriteSheet, 1.0f);
	Karem::Renderer2D::SubmitQuad(transform, {0.3f, 0.0f, 0.9f, 1.0f });
	Karem::Renderer2D::SubmitQuad(glm::translate(glm::mat4(1.0f), {0.0f, -5.0, 0.2f}) * glm::scale(glm::mat4(1.0f), { 3.0f, 3.0f, 1.0f }), m_SpriteSheet, 1.0f);
	for (float i = 0; i < 10; i += 1.0f)
	{
		for (float j = 0; j < 10; j += 1.0f)
		{
			float red = i / 10.0f, green = j / 10.0f;
			const glm::vec4 color = { red, green, 0.5f, 1.0f };
			Karem::Renderer2D::SubmitQuad({ i, j, -0.1f, 1.0f }, { 3.0f, 3.0f }, color);
		}
	}
	Karem::Renderer2D::EndScene();
#endif
	
	m_ActiveScene.Update(ts);
	m_FrameBuffer->UnBind();
}

void SandboxLayer::RenderImGUI()
{
	imgui::BeginFrame();


	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	const ImGuiWindowClass* window_class = NULL;

	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	char label[32];
	imgui::ImFormatString(label, IM_ARRAYSIZE(label), "DockSpaceViewport_%08X", viewport->ID);

	ImGuiWindowFlags host_window_flags = 0;
	host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking;
	host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		host_window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::Begin(label, NULL, host_window_flags);
	ImGuiID dockspace_id = ImGui::GetID("DockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags, window_class);
	ImGui::End();

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	ImGui::Begin("main");
	ImVec2 currentPannelSize = ImGui::GetContentRegionAvail();
	{
		const auto& [fbWidth, fbHeight] = m_FrameBuffer->GetFrameBufferSize();
		if (currentPannelSize.x != fbWidth or currentPannelSize.y != fbHeight)
			m_FrameBuffer->Resize((int32_t)currentPannelSize.x, (int32_t)currentPannelSize.y);
	}

	ImGui::Image(
		(void*)m_FrameBuffer->GetTextureColorAttachmentID(),
		{ (float)currentPannelSize.x, (float)currentPannelSize.y },
		ImVec2(0, 1),
		ImVec2(1, 0)
	);
	ImGui::End();

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	ImGui::PopStyleVar(3);

	m_ActiveScene.RenderImGUI();

	imgui::EndFrame();
}

void SandboxLayer::EventHandler(Karem::Event& event)
{
	Karem::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Karem::WindowResizeEvent>(std::bind(&SandboxLayer::WindowResizeAction, this, std::placeholders::_1));
	m_ActiveScene.EventHandler(event);
	//m_Camera.EventHandler(event);
}

bool SandboxLayer::WindowResizeAction(Karem::WindowResizeEvent& event)
{
	// set camera zoom level
	float zoom = (float)event.GetWidth() / (float)event.GetHeight();
	//ENGINE_DEBUG("{}", zoom);
	//m_Camera.SetZoom(zoom);
	return true;
}