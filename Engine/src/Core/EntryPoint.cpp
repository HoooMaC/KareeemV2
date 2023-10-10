#include "Core/Kareeem.h"
#include "Core/Application.h"

int main()
{
	Karem::Application* application = Karem::CreateApplication();
	application->Run();
	delete application;
}