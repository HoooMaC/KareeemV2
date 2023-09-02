#include "Core/Kareeem.h"

#include "RenderCommand.h"

#include "API/OpenGL/OpenGLRendererAPI.h"

namespace Karem {

	std::unique_ptr<RendererAPI> RendererCommand::s_RendererAPI = std::make_unique<OpenGLRendererAPI>();

}