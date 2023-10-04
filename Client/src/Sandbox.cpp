#include "Sandbox.h"

Karem::Application* Karem::CreateApplication()
{
	return new Sandbox();
}



Sandbox::Sandbox()
	: Application({ "Sandbox Application", 1280, 720})
{
	Init();

	// this is temporary
	layer = std::make_shared<SandboxLayer>(&m_Window);

	m_Layers.PushLayer(layer);
}