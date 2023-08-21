#include "Sandbox.h"

#include <glad/glad.h>

Karem::Application* Karem::CreateApplication()
{
	return new Sandbox();
}

void AppLayer::OnUpdate()
{
}
