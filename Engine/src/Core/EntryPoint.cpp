#include "Core/Kareeem.h"
#include "Core/Application.h"

#include <GLFW/glfw3.h>

int main()
{
	Karem::Application* application = new Karem::Application();

	application->Run();

	delete application;
}