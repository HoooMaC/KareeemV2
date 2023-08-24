#pragma once

#include "KaremEngine.h"

#include "ControlLayer.h"

class AppLayer : public Karem::Layer
{
public:
	AppLayer(const std::string& layerName = "Application")
		: Layer(layerName) {}

	void OnAttach() override;
	void OnUpdate() override;

private:
	unsigned int m_VertexBuffer, m_VertexArray, m_IndexBuffer, m_ShaderProgram;
};

class Sandbox : public Karem::Application
{
public:
	Sandbox();
	~Sandbox();

	void Run() override;
	void EventHandler(Karem::Event& event) override;
protected:
	void Init() override;
	void Shutdown() override;

private:
	bool WindowCloseAction(Karem::WindowCloseEvent& event);
	
	void PushLayer(std::shared_ptr<Karem::Layer> layer);
	void PushOverlay(std::shared_ptr<Karem::Layer> overlay);
	void PopLayer(std::shared_ptr<Karem::Layer> layer);
	void PopOverlay(std::shared_ptr<Karem::Layer> overlay);
private:
	ControlLayer m_ControlLayer;
	Karem::Layers m_Layers;
};

