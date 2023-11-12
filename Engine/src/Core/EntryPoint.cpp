#include "Core/Kareeem.h"
#include "Core/Application.h"

int main()
{
	Karem::StartUpApplication(Karem::AppType::GameEngine);
	Karem::RunApplication();
	Karem::ShutdownApplication();
}