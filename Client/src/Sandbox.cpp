#include "Sandbox.h"

//#include "TriangleLayer.h"

//#include <glad/glad.h>

// TEMP : THIS SHOULD NOT BE HERE (MAYBE)
#include "imgui.h"
//#include "imgui_setup.h"
//#include "backends/imgui_impl_glfw.h"
//#include "backends/imgui_impl_opengl3.h"

static constexpr int32_t appWidth = 1280, appHeight = 720;

Karem::Application* Karem::CreateApplication()
{
	return new Sandbox();

}

void SandboxLayer::OnAttach()
{
	// this is overriding texture in index 3 in texture renderer
	m_Texture = Karem::CreateTexture2D("res/texture/spritesheet/city_tilemap.png", 1);
	m_SpriteSheet = Karem::CreateSubTexture(m_Texture, { 0,1 }, { 8,8 }, { 3,3 });
	m_Camera = Karem::OrthographicCamera((float)appWidth / appHeight);
	m_FrameBuffer = Karem::CreateFrameBuffer(1280, 720);
}

void SandboxLayer::OnDetach()
{
}

void SandboxLayer::OnUpdate(Karem::TimeStep ts)
{

	m_Camera.OnUpdate(ts);

	m_FrameBuffer->Bind();

	Karem::RendererCommand::Clear();
	Karem::RendererCommand::ClearColor("#026773");

	Karem::Renderer2D::BeginScene(m_Camera);
	Karem::Renderer2D::SubmitSubTexturedQuad({ -1.0f, -1.0f, 0.0f }, { 3.0f, 3.0f }, m_SpriteSheet, 1.0f);

	for (float i = 0; i < 10; i += 1.0f)
	{
		for (float j = 0; j < 10; j += 1.0f)
		{
			float red = i / 10.0f, green = j / 10.0f;
			const glm::vec4 color = { red, green, 0.5f, 1.0f };
			Karem::Renderer2D::SubmitQuad({ i, j, -0.1f }, { 3.0f, 3.0f }, color);
		}
	}
	Karem::Renderer2D::EndScene();
	m_FrameBuffer->UnBind();
}

void SandboxLayer::OnImGUIRender()
{
	imgui::BeginFrame();

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	const ImGuiWindowClass* window_class = NULL;

	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGuiWindowFlags host_window_flags = 0;
	host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking;
	host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		host_window_flags |= ImGuiWindowFlags_NoBackground;

	char label[32];
	imgui::ImFormatString(label, IM_ARRAYSIZE(label), "DockSpaceViewport_%08X", viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin(label, NULL, host_window_flags);
	ImGui::PopStyleVar(3);

	ImGuiID dockspace_id = ImGui::GetID("DockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags, window_class);
	ImGui::End();
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	ImGui::Begin("main");
	ImVec2 imageSize = ImGui::GetContentRegionAvail();
	{
		auto [width, height] = m_FrameBuffer->GetFrameBufferSize();
		if (imageSize.x != width or imageSize.y != height)
		{
			m_FrameBuffer->Resize(imageSize.x, imageSize.y);
		}
	}

	ImGui::Image(
		(void*)m_FrameBuffer->GetTextureColorAttachmentID(),
		{ imageSize.x, imageSize.y },
		ImVec2(0, 1),  // Koordinat bawah kiri pada tekstur
		ImVec2(1, 0)   // Koordinat atas kanan pada tekstur
	);
	ImGui::End();

	imgui::EndFrame();
}

void SandboxLayer::EventHandler(Karem::Event& event)
{
	m_Camera.OnEvent(event);
}

Sandbox::Sandbox()
	: Application({ "Sandbox Application", 1280, 720})
{
	Init();

	// this is temporary
	layer = std::make_shared<SandboxLayer>();

	m_Layers.PushLayer(layer);
}

void Sandbox::Init()
{

}

void Sandbox::Shutdown()
{

}