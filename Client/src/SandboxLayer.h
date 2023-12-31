#pragma once

#include "KaremEngine.h"

class SandboxLayer : public Karem::Layer
{
public:
	SandboxLayer()
		: Layer("Sandbox Layer") {}
	SandboxLayer(Karem::Window* window)
		: Layer("Sandbox Layer"), m_ApplicationWindow(window) {}

	void Update(Karem::TimeStep ts) override;

	void OnAttach() override;
	void OnDetach() override;

	void RenderImGUI() override;
	void EventHandler(Karem::Event& event) override;
private:
	bool WindowResizeAction(Karem::WindowResizeEvent& event);


private:
	Karem::OrthographicCamera m_Camera;
	Karem::Window* m_ApplicationWindow = nullptr;
	Karem::Scene m_Scene;

	std::shared_ptr<Karem::Texture2D> m_Texture;
	std::shared_ptr<Karem::SubTexture2D> m_SpriteSheet;
	std::shared_ptr<Karem::FrameBuffer> m_FrameBuffer;
};
