#include "Core/Kareeem.h"
#include "Core/Application.h"

#include <GLFW/glfw3.h>

int main()
{
	ENGINE_INFO("Engine logging succesfully");

	Karem::Application* application = new Karem::Application();

	application->Run();

	delete application;
}