#pragma once

// TEMPORARY 
struct GLFWwindow;

namespace Karem {

	class Platform
	{
	public:
		static float GetTime();
		static void ResizeWindow(GLFWwindow* window, int width, int height);
	};

}
