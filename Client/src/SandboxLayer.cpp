#include "SandboxLayer.h"

#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Scene/Components.h"

#include <imgui.h>
#include <imgui_setup.h>

static constexpr int32_t appWidth = 1280, appHeight = 720;

void SandboxLayer::OnAttach()
{
	// this is overriding texture in index 3 in texture renderer
	m_Texture = Karem::CreateTexture2D("res/texture/spritesheet/city_tilemap.png", 1);
	m_SpriteSheet = Karem::SubTexture2D(m_Texture, { 0,4 }, { 8,8 }, { 5,4 });
	m_FrameBuffer = Karem::CreateFrameBuffer(1280, 720);
	m_Camera = Karem::OrthographicCamera({ 16,9 }, 1.0f);

	Karem::Entity cameraEntity = m_ActiveScene.CreateEntity("Camera");
	auto& cameraComponent = cameraEntity.AddComponent<Karem::CameraComponent>(Karem::OrthographicCamera({ 16,9 }, 1));
	auto& camera = cameraComponent.Camera;

	camera.SetPerspectiveCamera(Karem::PerspectiveCamera(1.77, 45.0f, 0.01, 100.0f));
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
	host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_MenuBar;
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		host_window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::Begin(label, NULL, host_window_flags);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			ShowExampleMenuFile();
			ImGui::Separator();
			if (ImGui::MenuItem("Quit", "Alt+F4"))
				SandboxLayer::CloseWindow();
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

	ImGuiID dockspace_id = ImGui::GetID("DockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags, window_class);
	ImGui::End();

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



	ImGui::Begin("main");
	ImVec2 currentPannelSize = ImGui::GetContentRegionAvail();
	{
		const auto& [fbWidth, fbHeight] = m_FrameBuffer->GetFrameBufferSize();

		// TO DO : CHANGE THIS ACCORDING TO THE CAMERA
		if (currentPannelSize.x != fbWidth or currentPannelSize.y != fbHeight)
		{
			m_FrameBuffer->Resize((int32_t)currentPannelSize.x, (int32_t)currentPannelSize.y);
			// also resize the camera
			// we are in the layer class
			// how to get the cameara component from registry which inside the scene class.
			// And the scene class is the data member of layer
		}
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
