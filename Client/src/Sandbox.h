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
	Sandbox() 
	{
		m_Layers.PushLayer(std::make_shared<AppLayer>());
		m_Layers.PushLayer(std::make_shared<Karem::ImGUILayer>());
	}
};

