#include "Core/Kareeem.h"
#include "Core/Application.h"

#include <GLFW/glfw3.h>

int main()
{
	int succes = glfwInit();
	if (!succes)
		std::cout << "failed to initialize GLFW\n";

	Karem::Application* app = Karem::CreateApplication();
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Test Window", NULL, NULL);
	if (!window)
	{
		std::cout << "Error\n";
		std::cin.get();
	}
	app->Run();

	glfwTerminate();
}