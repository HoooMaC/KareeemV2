#include "Core/Kareeem.h"
#include "Core/Application.h"

#include <GLFW/glfw3.h>

int main()
{
	int succes = glfwInit();
	if (!succes)
		std::cout << "failed to initialize GLFW\n";

	Karem::Application* app = Karem::CreateApplication();
	app->Run();

	glfwTerminate();
}