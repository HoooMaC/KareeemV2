#include "Core/Kareeem.h"

#include "BaseShader.h"

#include "API/OpenGL/OpenGLShader.h"

namespace Karem {

	std::shared_ptr<Shader> Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
	{
		return std::make_shared<OpenGLShader>(vertexShader, fragmentShader);
	}

}