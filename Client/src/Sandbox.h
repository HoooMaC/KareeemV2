#pragma once

#include "SandboxLayer.h"

class Sandbox : public Karem::Application
{
public:
	Sandbox();
	~Sandbox() = default;

private:
	std::shared_ptr<Karem::Layer> layer;
};

