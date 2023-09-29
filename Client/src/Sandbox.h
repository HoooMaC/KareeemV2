#pragma once

#include "KaremEngine.h"

class SandboxLayer : public Karem::Layer
{
public:
	SandboxLayer()
		: Layer("Sandbox Layer") {}
	void OnUpdate(Karem::TimeStep ts) override;

	void OnAttach() override;
	void OnDetach() override;

	void OnImGUIRender() override;
	void EventHandler(Karem::Event& event) override;
private:
	Karem::OrthographicCamera m_Camera;

	std::shared_ptr<Karem::Texture2D> m_Texture;
	std::shared_ptr<Karem::SubTexture2D> m_SpriteSheet;
	std::shared_ptr<Karem::FrameBuffer> m_FrameBuffer;
};

class Sandbox : public Karem::Application
{
public:
	Sandbox();
	~Sandbox() = default;

protected:
	void Init() override;
	void Shutdown() override;

private:
	std::shared_ptr<Karem::Layer> layer;
};

