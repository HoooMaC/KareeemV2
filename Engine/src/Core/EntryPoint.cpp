#include "Core/Kareeem.h"
#include "Core/Application.h"

int main()
{
	Karem::StartUpApplication(Karem::AppType::GameEngine);
	Karem::RunApplication();
	Karem::ShutdownApplication();

	//Karem::Application::StartUp(Karem::AppType::GameEngine);
	//Karem::Application::Run();
	//Karem::Application::Shutdown();
}