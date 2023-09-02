#pragma once

#include "Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Karem {

	class OpenGLContext : public GraphicsContext
	{
	public:
		void Init() override;
		static void SwapBuffers(GLFWwindow* window);
	};

}
