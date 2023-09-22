#pragma once

#include "KaremEngine.h"

class SandboxLayer : public Karem::Layer
{
public:
	SandboxLayer()
		: Layer("Sandbox Layer") {}
	void OnUpdate(Karem::TimeStep ts) override;
	void OnAttach() override;
private:
	std::shared_ptr<Karem::Texture2D> m_Texture;
	std::shared_ptr<Karem::SubTexture2D> m_SpriteSheet;
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

