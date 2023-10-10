#include "Core/Kareeem.h"

#include "BaseShader.h"

#include "Renderer/BaseRendererAPI.h"

#include "API/OpenGL/OpenGLShader.h"

namespace Karem {

	std::shared_ptr<Shader> CreateShader(std::string_view vertexShader, std::string_view fragmentShader)
	{
		switch (RendererAPI::GetAPI())
		{
			case API::OpenGL: return std::make_shared<OpenGLShader>(vertexShader, fragmentShader);
		}
		ENGINE_ASSERT(false, "Renderer API::NONE hasn't supported yet");
		return nullptr;
	}

}