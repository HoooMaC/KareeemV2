#include "Core/Kareeem.h"
#include "Core/Application.h"

#include <GLFW/glfw3.h>

int main()
{
	ENGINE_INFO("Engine logging succesfully");

	Karem::Application* application = Karem::CreateApplication();

	application->Run();

	delete application;
}