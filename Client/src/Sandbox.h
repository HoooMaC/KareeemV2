#pragma once

#include "KaremEngine.h"
class AppLayer : public Karem::Layer
{
public:
	AppLayer(const std::string& layerName = "Application")
		: Layer(layerName) {}
};

class Sandbox : public Karem::Application
{
public:
	Sandbox() 
	{
		m_Layers.PushLayer(std::make_shared<AppLayer>());
	}
};

