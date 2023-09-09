#include "Core/Kareeem.h"

#include "OpenGLShader.h"

#include <glad/glad.h>

namespace Karem {

	OpenGLShader::OpenGLShader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
	{
		std::string vertexShader = ReadFile(vertexShaderFilePath);
		std::string fragmentShader = ReadFile(fragmentShaderFilePath);

		const char* vertexShaderSource = vertexShader.c_str();
		const char* fragmentShaderSource = fragmentShader.c_str();

		GLuint vertexShaderId = CompileShader(vertexShaderSource, GL_VERTEX_SHADER);
		GLuint fragmentShaderId = CompileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

		CreateShader(vertexShaderId, fragmentShaderId);
		
		ENGINE_DEBUG("Creating a shader with id [{}]", m_RendererID);
	}


	void OpenGLShader::CreateShader(uint32_t vertexShader, uint32_t fragmentShader)
	{
		m_RendererID = glCreateProgram();
		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);
		glLinkProgram(m_RendererID);

		// TO DO : need to check the link status
		int success;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetProgramInfoLog(m_RendererID, 512, nullptr, infoLog);
			ENGINE_ERROR("Error linking shader program:");
			glDeleteProgram(m_RendererID);
			return;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	uint32_t OpenGLShader::CompileShader(const std::string& source, uint32_t type)
	{
		uint32_t shaderId = glCreateShader(type);
		const char* shaderSource = source.c_str();
		glShaderSource(shaderId, 1, &shaderSource, nullptr);
		glCompileShader(shaderId);

		// TODO: Check the compilation status
		int success;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
			ENGINE_ERROR("Error compiling shader :\n{}", infoLog);
			glDeleteShader(shaderId);
			return 0; // or appropriate error value
		}

		return shaderId;
	}


	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
		ENGINE_DEBUG("Binding shader with id [{}]", m_RendererID);
	}

	void OpenGLShader::UnBind() const
	{
		glUseProgram(0);
	}

	std::string OpenGLShader::ReadFile(const std::string& fileSource)
	{
		std::ifstream input(fileSource);

		if (!input.is_open())
		{
			// TODO : make some Assertion here
			ENGINE_ASSERT(false, "FILE IS NOT FOUND");
			return "";
		}

		std::stringstream buffer;
		buffer << input.rdbuf();

		//ENGINE_DEBUG("{}", buffer.str());

		input.close();

		return buffer.str();
	}

	OpenGLShader::~OpenGLShader()
	{
		Clear();
	}

	void OpenGLShader::Clear()
	{
		glDeleteProgram(m_RendererID);
	}
}