#include "Core/Kareeem.h"
#include "Core/Application.h"

#include <GLFW/glfw3.h>

int main()
{
	int succes = glfwInit();
	if (!succes)
		std::cout << "failed to initialize GLFW\n";

	//Karem::Application* app = Karem::CreateApplication();

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Test Window", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.8f, 0.3f, 0.4f, 1.0f);

		glfwSwapBuffers(window);

		glfwPollEvents();
		//std::cout << "The Application is Running in Engine \n";
	}

	//app->Run();

	glfwTerminate();
}