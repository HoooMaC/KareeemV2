#pragma once

struct GLFWwindow;

namespace Karem {

	class GraphicsContext
	{
	public:
		virtual void Init() = 0;
		static GLFWwindow* GetContextCurrent();
	};

}
