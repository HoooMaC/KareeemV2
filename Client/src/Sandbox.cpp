#include "Sandbox.h"

//#include "TriangleLayer.h"

//#include <glad/glad.h>

// TEMP : THIS SHOULD NOT BE HERE (MAYBE)
#include "imgui.h"
//#include "imgui_setup.h"
//#include "backends/imgui_impl_glfw.h"
//#include "backends/imgui_impl_opengl3.h"

Karem::Application* Karem::CreateApplication()
{
	return new Sandbox();
}

void SandboxLayer::OnAttach()
{
	// this is overriding texture in index 3 in texture renderer
	m_Texture = Karem::CreateTexture2D("res/texture/spritesheet/city_tilemap.png", 1);
	m_SpriteSheet = Karem::CreateSubTexture(m_Texture, { 0,1 }, { 8,8 }, { 3,3 });
}

void SandboxLayer::OnUpdate(Karem::TimeStep ts)
{
	Karem::Renderer2D::SubmitSubTexturedQuad({ 0.0f, 0.0f, 0.0f }, { 3.0f, 3.0f }, m_SpriteSheet, 1.0f);
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