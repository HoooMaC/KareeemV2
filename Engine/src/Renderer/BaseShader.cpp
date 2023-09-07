#include "Core/Kareeem.h"

#include "BaseShader.h"

#include "Renderer/BaseRendererAPI.h"

#include "API/OpenGL/OpenGLShader.h"

namespace Karem {

	std::shared_ptr<Shader> CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
	{
		switch (RendererAPI::GetAPI())
		{
			case API::None: ENGINE_ASSERT(false, "Renderer API::NONE hasn't supported yet") break;
			case API::OpenGL: return std::make_shared<OpenGLShader>(vertexShader, fragmentShader);
		}
	}

}