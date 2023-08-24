#pragma once

#include "KaremEngine.h"

class AppLayer : public Karem::Layer
{
public:
	AppLayer(const std::string& layerName = "Application")
		: Layer(layerName) {}

	void OnUpdate() override;
};

class Sandbox : public Karem::Application
{
public:
	Sandbox();

	void Run() override;
	void EventHandler(Karem::Event& event) override;
protected:
	void Init() override;
	void Shutdown() override;

private:
	bool WindowCloseAction(Karem::WindowCloseEvent& event);

private:
	Karem::Window m_Window;
	bool m_Running = true;
	Karem::Layers m_Layers;

private:
	unsigned int m_VertexBuffer, m_VertexArray, m_IndexBuffer, m_ShaderProgram;

};

